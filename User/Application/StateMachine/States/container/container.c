#include "../states.h"
#include "cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateContainer;
bool checkContainerCountdowns();

bool STATE_CONTAINER_SAVED = false;
int containerCountdownStart = 0;

void open_container(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // init converter
	servo_set_position(servo1, 800);
	HAL_Delay(300);
	servo_set_position(servo1, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}

void close_container(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // init converter
	servo_set_position(servo1, -1800);
	HAL_Delay(300);
	servo_set_position(servo1, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}


bool checkContainerCountdowns()
{
  // If the apogee countdown is finished, fire it
  if(containerCountdownStart > 0 && abs(containerCountdownStart - HAL_GetTick() ) >= CONTAINER_COUNTDOWN) {
    return true;
  }


  return false;
}


/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateContainerFunction(stateInput_t input, stateOutput_t* output)
{
    //log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State: Container");

	if (!STATE_CONTAINER_SAVED)
	{
		lastStateFileWrite(CONTAINER_CHAR);
		data.software_state = CONTAINER_CHAR-'0';
		STATE_CONTAINER_SAVED = true;
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (4) Container saved!");
	}



//	if (0/*data.altitude <= breakingLevel*/)  // set breaking lvl
//	{
//		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Container -> breaking successed!");
//		return &stateBreaking;
//	}


	if(containerCountdownStart == 0) containerCountdownStart = HAL_GetTick();


	if(checkContainerCountdowns())
	{
		open_container();
		return &stateAutogyro;
	}

	return &stateContainer;
}

