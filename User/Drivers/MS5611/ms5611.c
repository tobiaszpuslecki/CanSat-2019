#include "stm32f1xx_hal.h"
#include "ms5611.h"

#include <math.h>

static I2C_HandleTypeDef * I2C;
static MS5611_resolution resolution;
static uint16_t calibrate[6];

static int32_t dT;
static float pressureLevelZero;

/*
 * Function:  MS5611_calibrate
 * ---------------------------
 *  Populate "calibrate" array with factory calibrated values read from sensor's PROM
 *
 *  returns: result of operation as MS5611_result value
 */
static const MS5611_result MS5611_calibrate(void)
{
	uint8_t receive[2];

	for (uint8_t q = 0; q < 6; ++q)
	{
		receive[0] = MS5611_CMD_READ_PROM + 2 * q;

		if (HAL_I2C_Master_Transmit(I2C, MS5611_ADDRESS, receive, 1, MS5611_I2C_TIMEOUT) != HAL_OK)
			return MS5611_Error;
		if (HAL_I2C_Master_Receive(I2C, MS5611_ADDRESS, receive, 2, MS5611_I2C_TIMEOUT) != HAL_OK)
			return MS5611_Error;

		calibrate[q] = (receive[0] << 8) + receive[1];
	}
	return MS5611_OK;
}

/*
 * Function:  MS5611_requestData
 * -----------------------------
 *  Sends desired conversion command through I2C interface using appropriate HAL call
 *
 *  CMD: command to be sent 
 *
 *  returns: result of operation as MS5611_result value
 */
static const MS5611_result MS5611_requestData(uint8_t CMD)
{
	if (HAL_I2C_Master_Transmit(I2C, MS5611_ADDRESS, &CMD, 1, HAL_MAX_DELAY) != HAL_OK)
		return MS5611_Error;
	return MS5611_OK;
}

/*
 * Function:  MS5611_getData
 * -------------------------
 *  Reads ADC conversion data from sensor and stores it inside provided variable
 *
 *  data: pointer to desired data storing adress
 *
 *  returns: result of operation as MS5611_result value
 */
static const MS5611_result MS5611_getData(uint32_t * data)
{
	uint8_t receive[3];

	if (HAL_I2C_Master_Transmit(I2C, MS5611_ADDRESS, MS5611_CMD_ADC_READ, 1, MS5611_I2C_TIMEOUT) != HAL_OK)
		return MS5611_Error;

	if (HAL_I2C_Master_Receive(I2C, MS5611_ADDRESS, receive, 3, MS5611_I2C_TIMEOUT) != HAL_OK)
		return MS5611_Error;

	*data = (receive[0] << 16) + (receive[1] << 8) + receive[2];

	return MS5611_OK;
}

/*
 * Function:  MS5611_init
 * ----------------------
 *  Initializes MS5611 sensor
 *
 *  I2Cx: pointer to I2C handle Structure
 *  ms5611: pointer to MS5611 sensor Instance
 *  res: sensor resolution in hex
 *
 *  returns: result of operation as MS5611_result value
 */
const MS5611_result MS5611_init(I2C_HandleTypeDef * I2Cx, MS5611 * ms5611, const MS5611_resolution res)
{
	I2C	= I2Cx;

	ms5611->pressure = 0;
	ms5611->pressureRaw = 0;
	pressureLevelZero = MS5611_SEA_PRESSURE;

	ms5611->temperature = 0;
	ms5611->temperatureRaw = 0;

	resolution	= res;

	if( HAL_I2C_IsDeviceReady(I2C, MS5611_ADDRESS, 2, MS5611_I2C_TIMEOUT) != HAL_OK )
		return MS5611_Disconnected;

	if( MS5611_calibrate() != MS5611_OK )
		return MS5611_Error;

	return MS5611_OK;
}

/*
 * Function:  MS5611_requestTemperature
 * ------------------------------------
 *  Sends temperature conversion command to the sensor
 *
 *  returns: sensor digital temperature reading value
 */
const MS5611_result MS5611_requestTemperature( void )
{
	return MS5611_requestData( MS5611_CMD_CONV_D2 + resolution );
}

/*
 * Function:  MS5611_getTemperature
 * --------------------------------
 *  Reads raw temperature data from sensor, calculates actual temperature 
 *  and stores it inside "temperature" field of provided ms5611 Struct
 *
 *  ms5611: pointer to MS5611 sensor Instance
 *
 *  returns: result of operation as MS5611_result value
 */
const MS5611_result MS5611_getTemperature( MS5611 * ms5611 )
{
	if( MS5611_getData( &(ms5611->temperatureRaw) ) != MS5611_OK )
		return MS5611_Error;
	
	dT = ms5611->temperatureRaw - (calibrate[4] << 8 );														// Difference between actual and reference temperature
	ms5611->temperature = MS5611_REF_TEMPERATURE + ((int64_t)dT * calibrate[5] >> 23);						// Actual temperature

	if (ms5611->temperature < MS5611_REF_TEMPERATURE)
		ms5611->temperature -= (dT * dT) >> 31;

	return MS5611_OK;
}

/*
 * Function:  MS5611_requestPressure
 * ---------------------------------
 *  Sends pressure conversion command to the sensor
 *
 *  returns: sensor raw pressure reading value
 */
const MS5611_result MS5611_requestPressure( void )
{
	return MS5611_requestData( MS5611_CMD_CONV_D1 + resolution );
}

/*
 * Function:  MS5611_getPressure
 * -----------------------------
 *  Reads raw pressure data from sensor, calculates offset and sensitivity relative 
 *  to temperature and subsequently calculates and stores temperature compensated pressure 
 *  inside "pressure" field of provided ms5611 Struct
 *
 *  ms5611: pointer to MS5611 sensor Instance
 *
 *  returns: result of operation as MS5611_result value
 */
const MS5611_result MS5611_getPressure( MS5611 * ms5611 )
{
	if(MS5611_getData( &(ms5611->pressureRaw) ) != MS5611_OK)
		return MS5611_Error;
	
	uint64_t OFF = (uint32_t)(calibrate[1] << 16) + dT * (calibrate[3] >> 7);								// Offset at actual temperature
	uint64_t SENS = (uint32_t)(calibrate[0] << 15) + dT * (calibrate[2] >> 8);								// Sensitivity at actual temperature

	ms5611->pressure = ((SENS * ms5611->pressureRaw >> 21) - OFF);											// Temperature compensated pressure
	ms5611->pressure >>= 15;

	return MS5611_OK;
}

/*
 * Function:  MS5611_setLevelZero
 * ------------------------------
 *  Sets "pressureLevelZero" variable with current temperature compensated pressure value
 *
 *  ms5611: pointer to MA5611 sensor Instance 
 */
void MS5611_setLevelZero(MS5611 * ms5611)
{
	pressureLevelZero = ms5611->pressure;
}

/*
 * Function:  MS5611_calculateAltitude
 * -----------------------------------
 *  Calculates altitude using current temperature and temperature compensated pressure and stores it 
 *  inside "altitude" field of provided ms5611 Struct
 *
 *  ms5611: pointer to MA5611 sensor Instance 
 */
void MS5611_calculateAltitude( MS5611 * ms5611 )
{
	ms5611->altitude = BARO_CONST * (1.0 - pow(ms5611->pressure / pressureLevelZero, BARO_PWR_INDEX));		// Barometric formula
}
