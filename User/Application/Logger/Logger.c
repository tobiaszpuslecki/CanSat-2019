/**
    \Brief:     Logger_t struct implementation 

    \File:      StateMachine.c

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#include "stm32f1xx_hal.h"
#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

static const char* LOG_TYPE_PREFIX[] = {"INFO", "WARRNING", "ERROR", "CRITICAL"};
static const char* LOG_DEVICE_PREFIX[] = {"SYSTEM", "MPU9250", "MS5611", "INA219"};

static char log_buffer[LOG_BUFFER_SIZE];
static uint16_t log_length;


static bool log_sendBuffer()
{
	uint8_t fails = 0;

	if((LOG_DATABUS) & LOG_DATABUS_USB)
	{
		fails += CDC_Transmit_FS(log_buffer, log_length) != HAL_OK;
	}

	if((LOG_DATABUS) & LOG_DATABUS_USART)
	{
		fails += HAL_UART_Transmit(&LOG_UART_HANDLER,log_buffer,log_length, LOG_UART_TIMEOUT) != HAL_OK;
	}

	return fails;
}


bool log_data(log_type_t type, log_device_t device, const char* message, ...)
{
	if(!LOG_ENABLE)
	{
		return false;
	}

    va_list variables;
    uint8_t fails = 0;

    log_length = sprintf(log_buffer, "[%lu][%s][%s]:", HAL_GetTick(), LOG_TYPE_PREFIX[type], LOG_DEVICE_PREFIX[device]);
    fails += log_sendBuffer();

    va_start(variables, message);
    log_length = vsprintf(log_buffer, message, variables);
    va_end(variables);
    fails += log_sendBuffer();

    log_length = sprintf(log_buffer, "\n\r");
    fails += log_sendBuffer();

    return fails == 0;
}

