/*
 * gps.h
 *
 *  Created on: May 25, 2019
 *      Author: Jakub Pal
 */

#ifndef DRIVERS_GPS_GPS_H_
#define DRIVERS_GPS_GPS_H_

#include "usart.h"
#include "nmea.h"

extern gps_t hgps;

void ublox_init(UART_HandleTypeDef* huart);
void GPS_RxCpltCallback(UART_HandleTypeDef* huart);

#endif /* DRIVERS_GPS_GPS_H_ */
