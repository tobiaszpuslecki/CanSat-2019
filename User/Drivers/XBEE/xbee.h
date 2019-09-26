/*
 * xbee.h
 *
 *  Created on: May 28, 2019
 *      Author: Jakub Pal
 */

#ifndef DRIVERS_XBEE_XBEE_H_
#define DRIVERS_XBEE_XBEE_H_

#include "usart.h"
#include "cmd_parser.h"

typedef struct { // CDR page no 84
	uint32_t mission_time; //seconds from power up
	uint16_t packet_count; //frame no

	float altitude;
	uint32_t pressure;
	float temp;
	float voltage;

	/* GPS TIME */
	uint8_t gps_seconds;
	uint8_t gps_minutes;
	uint8_t gps_hours;

	/* GPS POSITION */
	float gps_latitude;
	float gps_longitude;
	float gps_altitude;
	uint8_t gps_sats;

	/* GYRO */
	float pitch;
	float roll;

	uint16_t blade_spin_rate;
	uint8_t software_state; // according to CDR page no 83
	uint16_t cam_direction;
} flight_data;

void xbee_init(UART_HandleTypeDef* huart);
void xbee_sendData(flight_data* data);

void XBEE_RxCpltCallback(UART_HandleTypeDef* huart);

#endif /* DRIVERS_XBEE_XBEE_H_ */
