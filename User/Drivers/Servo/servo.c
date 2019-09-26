/*
 * servo.c
 *
 *  Created on: 20.02.2019
 *      Author: Bartek Smaga
 */
#include "stm32f1xx_hal.h"

#include "servo.h"


static TIM_TypeDef *timer_handle;


void servos_init(TIM_TypeDef *timer)
{
	timer_handle = timer;
	servo_set_position(servo1, 0);    // Set to neutral position
	servo_set_position(servo2, 0);
}

void servo_set_position(enum e_servo_number nr, int16_t position)
{
	if( position > SERVO_POSITION_MAX )
	{
		position = SERVO_POSITION_MAX;
	}
	else if( position < SERVO_POSITION_MIN )
	{
		position = SERVO_POSITION_MIN;
	}

	position += SERVO_OFFSET;

	switch( nr )
	{
	case servo1:
		timer_handle->CCR1 = position;
		break;
	case servo2:
		timer_handle->CCR2 = position;
		break;
	}
}




