#ifndef CANSAT_INCLUDES_H
#define CANSAT_INCLUDES_H

#include "../../Application/StateMachine/StateMachine.h"
#include "../../Application/StateMachine/States/states.h"
#include <MS5611/ms5611.h>
#include <MPU9250/MPU9250.h>
#include <INA219/ina219.h>
#include <Servo/servo.h>
#include <GPS/gps.h>
#include <XBEE/xbee.h>
#include "Logger.h"
#include "../../User/Application/Reglin/coefficients.h"
#include "../../FATFS/App/fatfs.h"
#include "../Boot/cansat_boot.h"
#include <FileSystem/cansat_file_system.h>
#include <Data_Structure/data_struct.h>

#include <math.h>

#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#define PRELAUNCH_CHAR '1'
#define READY_CHAR '2'
#define FLIGHTUP_CHAR '3'
#define CONTAINER_CHAR '4'
#define AUTOGYRO_CHAR '5'
#define BREAKING_CHAR '6'
#define TOUCHDOWN_CHAR '7'
#define ERROR_CHAR 'X'

void open_container();
void close_container();

#endif /* CANSAT_INCLUDES_H */
