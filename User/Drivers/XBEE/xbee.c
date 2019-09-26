/*
 * xbee.c
 *jazon
 *  Created on: May 28, 2019
 *      Author: Jakub Pal
 */

#include "xbee.h"

#define PRINTF_DEFAULT_FLOAT_PRECISION 4
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T

#include "printf.h"

UART_HandleTypeDef* _xbee_uart = NULL;
uint8_t _received_char = 0;

cmd_t hcmd;

void xbee_init(UART_HandleTypeDef* huart){
	_xbee_uart = huart;
	HAL_UART_Receive_IT(_xbee_uart, &_received_char, 1);
}

void xbee_sendData(flight_data* data){
	char buffer[120];
	uint8_t len;

	char sw_state[15];
	switch (data->software_state){
	case 1:
		sprintf(sw_state, "PRELAUNCH");
		break;
	case 2:
		sprintf(sw_state, "READY");
		break;
	case 3:
		sprintf(sw_state, "FLIGHTUP");
		break;
	case 4:
		sprintf(sw_state, "CONTAINER");
		break;
	case 5:
		sprintf(sw_state, "AUTO-GYRO");
		break;
	case 6:
		sprintf(sw_state, "BREAKING");
		break;
	case 7:
		sprintf(sw_state, "TOUCHDOWN");
		break;
	default:
		sprintf(sw_state, "FAIL");
		break;
	}

	len = snprintf(buffer, 120, "4225,%lu,%d,%.1f,%lu,%.1f,%.2f,%02d:%02d:%02d,%.4f,%.4f,%.1f,%d,%.1f,%.1f,%d,%s,%d\r\n",
		          data->mission_time,
				  data->packet_count,
				  data->altitude,
				  data->pressure,
				  data->temp/100,
				  data->voltage,
				  data->gps_hours,
				  data->gps_minutes,
				  data->gps_seconds,
				  data->gps_latitude,
				  data->gps_longitude,
				  data->gps_altitude,
				  data->gps_sats,
				  data->pitch,
				  data->roll,
				  data->blade_spin_rate,
				  sw_state,
				  data->cam_direction);
	HAL_UART_Transmit(_xbee_uart, (uint8_t*)buffer, len, HAL_MAX_DELAY);
}

void XBEE_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart == _xbee_uart){
		process_cmd(&hcmd, &_received_char, 1);
		HAL_UART_Receive_IT(_xbee_uart, &_received_char, 1);
	}
}
