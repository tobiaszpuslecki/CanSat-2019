/**
    \Brief:     Logger_t struct header

    \File:      StateMachine.h

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#ifndef INCLUDE_LOGGER_H
#define INCLUDE_LOGGER_H

#include <stdbool.h>
#include "usart.h"

#define LOG_DATABUS_USB		0x01
#define LOG_DATABUS_USART	0x02

/////////////////////////////////////////////////
// LOG CONFIG

#define LOG_ENABLE       	false
#define LOG_BUFFER_SIZE   	128

#define LOG_DATABUS			LOG_DATABUS_USART

#define LOG_UART_HANDLER	huart2
#define LOG_UART_TIMEOUT	10

/////////////////////////////////////////////////

typedef enum
{
    LOG_TYPE_INFO = 0,
    LOG_TYPE_WARRNING,
    LOG_TYPE_ERROR,
    LOG_TYPE_CRITICAL,
}log_type_t;

typedef enum
{
	LOG_DEVICE_SYSTEM = 0,
	LOG_DEVICE_MPU9250,
	LOG_DEVICE_MS5611,
	LOG_DEVICE_INA219,
}log_device_t;

bool log_data(log_type_t type, log_device_t device, const char* message, ...);


// Example usage
// log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Init successed!");
// log_data(LOG_TYPE_INFO, MS5611, "Temp: %d", temp);


#endif
