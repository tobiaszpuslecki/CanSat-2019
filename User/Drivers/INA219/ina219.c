/*
 * ina219.c
 *
 *  Created on: 05.04.2018
 *      Author: Bartek
 */
#include "stm32f1xx_hal.h"

#include "ina219.h"



#define INA219_I2C_TIMEOUT     100

#define INA219_ADDRESS     0b10000000

#define INA219_REG_CONFIGURATION		0x00
#define INA219_REG_SHUNTVOLTAGE			0x01
#define INA219_REG_BUS_VOLTAGE			0x02
#define INA219_REG_POWER				0x03
#define INA219_REG_CURRENT				0x04
#define INA219_REG_CALIBRATION			0x05


float INA219_currentLSB, INA219_powerLSB;  // LSB -> [mA] [mW]

static I2C_HandleTypeDef* i2c_handle;



HAL_StatusTypeDef reg_write(uint8_t adr, uint16_t data)
{
	uint8_t buf[2];
	buf[0] = data >> 8;
	buf[1] = data;
    return HAL_I2C_Mem_Write(i2c_handle, INA219_ADDRESS, adr, 1, buf, 2, INA219_I2C_TIMEOUT);
}

HAL_StatusTypeDef reg_read(uint8_t adr, uint16_t *data)
{
	HAL_StatusTypeDef st;
	uint8_t buf[2];
	st = HAL_I2C_Mem_Read(i2c_handle, INA219_ADDRESS, adr, 1, buf, 2, INA219_I2C_TIMEOUT);
	*data = (uint16_t)buf[0] << 8;
	*data |= (uint16_t)buf[1];
	return st;
}

void INA219_Init(I2C_HandleTypeDef* i2c)
{
	i2c_handle = i2c;
}

HAL_StatusTypeDef INA219_configure(ina219_range_t range, ina219_gain_t gain, ina219_busRes_t busRes, ina219_shuntRes_t shuntRes, ina219_mode_t mode)
{
    uint16_t config = 0;

    config |= (range << 13 | gain << 11 | busRes << 7 | shuntRes << 3 | mode);

    return reg_write(INA219_REG_CONFIGURATION, config);
}

// Kalibracja
// ShuntValue - wartoœæ rezystora np 0.1 Ohm
// MaxExpectedCurren - oczekiwana maksymalna wartoœæ mierzonego pr¹du (dla GAIN=320mV I = 0.32/0.1 = 3.2A
HAL_StatusTypeDef INA219_calibrate(float ShuntValue, float MaxExpectedCurren)
{
    uint16_t val = 0;

    INA219_currentLSB = (MaxExpectedCurren*1000) / 32768;
    INA219_powerLSB = INA219_currentLSB * 20;
    val = (uint16_t)((40.96) / (INA219_currentLSB * ShuntValue));

    return reg_write(INA219_REG_CALIBRATION, val);
}


// Odczytaj napiêcie na boczniku
// Odczytana wartoœæ: x = x/100 mV = x/100000 V   ( LSB = 10µV )
HAL_StatusTypeDef INA219_readShuntVoltage_raw(uint16_t *value)
{
	return reg_read(INA219_REG_SHUNTVOLTAGE, value);
}

// Odczytaj napiêcie zasilania
// Odczytana wartoœæ: x = x*4 mV = x/250 V   ( LSB = 4mV )
HAL_StatusTypeDef INA219_readBusVoltage_raw(uint16_t *value)
{
	HAL_StatusTypeDef st = reg_read(INA219_REG_BUS_VOLTAGE, value);
	*value >>= 3;
	return st;
}

// Odczytaj napiêcie na boczniku
HAL_StatusTypeDef INA219_readShuntVoltage_mV(float *value)
{
	uint16_t v;
	HAL_StatusTypeDef st = INA219_readShuntVoltage_raw(&v);
	*value = v / 100.00;
	return st;
}

// Odczytaj napiêcie zasilania
HAL_StatusTypeDef INA219_readBusVoltage_mV(uint16_t *value)
{
	uint16_t v;
	HAL_StatusTypeDef st = INA219_readBusVoltage_raw(&v);
	*value = v * 4;
	return st;
}

// Odczytaj obliczon¹ moc ( LSB = powerLSB mW )
HAL_StatusTypeDef INA219_readPower_raw(uint16_t *value)
{
	return reg_read(INA219_REG_POWER, value);
}

// Odczytaj obliczony pr¹d ( LSB = currentLSB mA )
HAL_StatusTypeDef INA219_readCurrent_raw(uint16_t *value)
{
	return reg_read(INA219_REG_CURRENT, value);
}

// Odczytaj obliczon¹ moc [mW]
HAL_StatusTypeDef INA219_readPower_mW(float *value)
{
	uint16_t v;
	HAL_StatusTypeDef st = INA219_readPower_raw(&v);
	*value = v * INA219_powerLSB;
	return st;
}

// Odczytaj obliczony pr¹d [mA]
HAL_StatusTypeDef INA219_readCurrent_mA(float *value)
{
	uint16_t v;
	HAL_StatusTypeDef st = INA219_readCurrent_raw(&v);
	*value = v * INA219_currentLSB;
	return st;
}






