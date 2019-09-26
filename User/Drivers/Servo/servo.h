/*
 * servo.h
 *
 *  Created on: 20.02.2019
 *      Author: Bartek Smaga
 */

#ifndef APPLICATION_USER_SERVO_H_
#define APPLICATION_USER_SERVO_H_

#include "stm32f1xx_hal.h"


// ################################
// #############################################################

// Control of two servos using a timer which is set to PWM mode
// The timer should be set so that it generates a 50Hz PWM signal

// #############################################################
// ################################

// Timer parameters needed for calculations.
// F_PWM = TIMER_CLK / ( (ARR+1)*(PSC+1)*(CKD+1) )
// F_PWM should be about 50 Hz
//#define SERVO_TIMER_CLK		72000000UL		  // Main frequency of the timer
#define SERVO_TIMER_CLK		64000000UL		  // Main frequency of the timer
#define SERVO_PSC			18				 // Set preskaler
#define SERVO_CKD			0				// Set division

// Setting the control signal
//  _____
// |     |____________________________
//  <------------- T ---------------->
#define SERVO_MINIMUM_ANGLE		0.6   // [ms]  Minimum pulse duration for a minimum angle
#define SERVO_MAXIMUM_ANGLE		2.3  // [ms]   Maximum pulse duration for a maximum angle

// ################################
// #############################################################

#define SERVO_PWM_MIN		( SERVO_MINIMUM_ANGLE * SERVO_TIMER_CLK / (1000UL*(SERVO_PSC+1)*(SERVO_CKD+1)) )
#define SERVO_PWM_MAX		( SERVO_MAXIMUM_ANGLE * SERVO_TIMER_CLK / (1000UL*(SERVO_PSC+1)*(SERVO_CKD+1)) )

#define SERVO_OFFSET 		( (SERVO_PWM_MAX + SERVO_PWM_MIN) / 2 )

#define SERVO_POSITION_MIN	( (int16_t)SERVO_PWM_MIN - (int16_t)SERVO_OFFSET )
#define SERVO_POSITION_MAX	( (int16_t)SERVO_PWM_MAX - (int16_t)SERVO_OFFSET )

// Calculation of the control range:
// For:
// SERVO_TIMER_CLK		72 MHz
// SERVO_PSC			21
// SERVO_CKD			0
// SERVO_MINIMUM_ANGLE		0.6 ms
// SERVO_MAXIMUM_ANGLE		2.3 ms
//
// SERVO_PWM_MIN = 1963
// SERVO_PWM_MAX = 7527
// SERVO_OFFSET  = 4745
//
// Result:
// SERVO_POSITION_MIN  = -2782
// SERVO_POSITION_MAX  = +2782
//

enum e_servo_number { servo1, servo2 };


// Initialization the servos control
void servos_init(TIM_TypeDef *timer);

// Setting the servo position
// nr	    -> Servo number
//             This parameter can be one of the following values:
//             - servo1: TIM Channel 1 selected
//             - servo2: TIM Channel 2 selected
// position -> from -2782 to +2782 ( SERVO_POSITION_MIN and SERVO_POSITION_MAX values )
void servo_set_position(enum e_servo_number nr, int16_t position);


// #############################################################
// ################################


// Example code:
//
//	int pos = 0;
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
//	servos_init(TIM1);
//
//	while (1)
//	{
//		servo_set_position(servo1, pos);
//		servo_set_position(servo2, pos);
//		if( SERVO_POSITION_MAX == pos ) HAL_Delay(600);
//		if( SERVO_POSITION_MIN == pos  ) HAL_Delay(600);
//		pos += 1;
//		if(pos > SERVO_POSITION_MAX) pos = SERVO_POSITION_MIN;
//		HAL_Delay(15);
//	}
//

#endif /* APPLICATION_USER_SERVO_H_ */



