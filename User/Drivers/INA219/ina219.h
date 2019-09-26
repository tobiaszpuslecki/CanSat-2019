/*
 * ina219.h
 *
 *  Created on: 05.04.2018
 *      Author: Bartek
 */

#ifndef INA219_H_
#define INA219_H_

#include "stm32f1xx_hal.h"




typedef enum
{
    INA219_RANGE_16V            = 0b0,
    INA219_RANGE_32V            = 0b1
} ina219_range_t;

typedef enum
{
    INA219_GAIN_40MV            = 0b00,
    INA219_GAIN_80MV            = 0b01,
    INA219_GAIN_160MV           = 0b10,
    INA219_GAIN_320MV           = 0b11
} ina219_gain_t;

typedef enum
{
    INA219_BUS_RES_9BIT         = 0b0000,
    INA219_BUS_RES_10BIT        = 0b0001,
    INA219_BUS_RES_11BIT        = 0b0010,
    INA219_BUS_RES_12BIT        = 0b0011
} ina219_busRes_t;

typedef enum
{
    INA219_SHUNT_RES_9BIT_1S    = 0b0000,
    INA219_SHUNT_RES_10BIT_1S   = 0b0001,
    INA219_SHUNT_RES_11BIT_1S   = 0b0010,
    INA219_SHUNT_RES_12BIT_1S   = 0b0011,
    INA219_SHUNT_RES_12BIT_2S   = 0b1001,
    INA219_SHUNT_RES_12BIT_4S   = 0b1010,
    INA219_SHUNT_RES_12BIT_8S   = 0b1011,
    INA219_SHUNT_RES_12BIT_16S  = 0b1100,
    INA219_SHUNT_RES_12BIT_32S  = 0b1101,
    INA219_SHUNT_RES_12BIT_64S  = 0b1110,
    INA219_SHUNT_RES_12BIT_128S = 0b1111
} ina219_shuntRes_t;

typedef enum
{
    INA219_MODE_POWER_DOWN      = 0b000,
    INA219_MODE_SHUNT_TRIG      = 0b001,
    INA219_MODE_BUS_TRIG        = 0b010,
    INA219_MODE_SHUNT_BUS_TRIG  = 0b011,
    INA219_MODE_ADC_OFF         = 0b100,
    INA219_MODE_SHUNT_CONT      = 0b101,
    INA219_MODE_BUS_CONT        = 0b110,
    INA219_MODE_SHUNT_BUS_CONT  = 0b111,
} ina219_mode_t;


float INA219_currentLSB, INA219_powerLSB;  // LSB -> [mA] [mW]


void INA219_Init(I2C_HandleTypeDef* i2c);
HAL_StatusTypeDef INA219_configure(ina219_range_t range, ina219_gain_t gain, ina219_busRes_t busRes, ina219_shuntRes_t shuntRes, ina219_mode_t mode);
HAL_StatusTypeDef INA219_calibrate(float ShuntValue, float MaxExpectedCurren);

HAL_StatusTypeDef INA219_readShuntVoltage_raw(uint16_t *value);
HAL_StatusTypeDef INA219_readBusVoltage_raw(uint16_t *value);
HAL_StatusTypeDef INA219_readShuntVoltage_mV(float *value);
HAL_StatusTypeDef INA219_readBusVoltage_mV(uint16_t *value);
HAL_StatusTypeDef INA219_readPower_raw(uint16_t *value);
HAL_StatusTypeDef INA219_readCurrent_raw(uint16_t *value);
HAL_StatusTypeDef INA219_readPower_mW(float *value);
HAL_StatusTypeDef INA219_readCurrent_mA(float *value);



#endif /* INA219_H_ */
