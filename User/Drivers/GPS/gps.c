/*
 * gps.c
 *
 *  Created on: May 25, 2019
 *      Author: Jakub Pal
 */

#include "gps.h"
#include "nmea.h"

UART_HandleTypeDef* _gps_uart = NULL;
uint8_t _received_char;

/* GPS handle  */
gps_t hgps;

void ublox_init(UART_HandleTypeDef* huart){
	_gps_uart = huart;
	HAL_UART_Receive_IT(_gps_uart, &_received_char, 1);
}

void GPS_RxCpltCallback(UART_HandleTypeDef* huart){
	if(huart == _gps_uart){
		gps_process(&hgps, &_received_char, 1);
		HAL_UART_Receive_IT(_gps_uart, &_received_char, 1);
	}
}
