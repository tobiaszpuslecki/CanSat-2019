#ifndef MS5611_H
#define MS5611_H

#include "stm32f1xx_hal.h"

#define MS5611_I2C_TIMEOUT				1

#define MS5611_ADDRESS					0x77 << 1

// Specific command hex values as in datasheet
#define MS5611_CMD_ADC_READ				0x00
#define MS5611_CMD_RESET				0x1E
#define MS5611_CMD_CONV_D1				0x40
#define MS5611_CMD_CONV_D2				0x50
#define MS5611_CMD_READ_PROM			0xA2

// Specific values
#define MS5611_REF_TEMPERATURE			2000												// 20 degrees Celsius, from datasheet
#define MS5611_SEA_PRESSURE				101325.0

// Hard values from barometric formula
#define BARO_CONST						44330.0
#define BARO_PWR_INDEX					0.1902949

typedef enum
{
	MS5611_OK,
	MS5611_Error,
	MS5611_Disconnected,
	MS5611_InvalidDevice
}MS5611_result;

typedef enum
{
    MS5611_ULTRA_HIGH_RES   = 0x08,
    MS5611_HIGH_RES         = 0x06,
    MS5611_STANDARD         = 0x04,
    MS5611_LOW_POWER        = 0x02,
    MS5611_ULTRA_LOW_POWER  = 0x00
}MS5611_resolution;

typedef struct
{
	uint32_t pressureRaw;
	uint32_t temperatureRaw;

	uint32_t pressure;
	int16_t temperature;

	float altitude;
}MS5611;

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
const MS5611_result MS5611_init(	I2C_HandleTypeDef * I2Cx,
									MS5611 * ms5611,
									const MS5611_resolution res);

/*
 * Function:  MS5611_requestTemperature
 * ------------------------------------
 *  Sends temperature conversion command to the sensor
 *
 *  returns: sensor digital temperature reading value
 */
const MS5611_result MS5611_requestTemperature(void);

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
const MS5611_result MS5611_getTemperature(MS5611 * ms5611);

/*
 * Function:  MS5611_requestPressure
 * ---------------------------------
 *  Sends pressure conversion command to the sensor
 *
 *  returns: sensor raw pressure reading value
 */
const MS5611_result MS5611_requestPressure(void);

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
const MS5611_result MS5611_getPressure(MS5611 * ms5611);

/*
 * Function:  MS5611_setLevelZero
 * ------------------------------
 *  Sets "pressureLevelZero" variable with current temperature compensated pressure value
 *
 *  ms5611: pointer to MA5611 sensor Instance 
 */
void MS5611_setLevelZero(MS5611 * ms5611);

/*
 * Function:  MS5611_calculateAltitude
 * -----------------------------------
 *  Calculates altitude using current temperature and temperature compensated pressure and stores it 
 *  inside "altitude" field of provided ms5611 Struct
 *
 *  ms5611: pointer to MA5611 sensor Instance 
 */
void MS5611_calculateAltitude( MS5611 * ms5611 );

#endif
