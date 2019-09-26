#include "../states.h"
#include "../../../Includes/cansat_includes.h"


/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateBreaking;

bool STATE_BREAKING_SAVED = false;
bool CONSTANT_ACCELERATION = false;
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateBreakingFunction(stateInput_t input, stateOutput_t* output)
{

    if (!STATE_BREAKING_SAVED)
	{
		lastStateFileWrite(BREAKING_CHAR);
		data.software_state = BREAKING_CHAR-'0';
		STATE_BREAKING_SAVED = true;
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (6) Breaking saved!");
	}

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // init converter
    servo_set_position(servo2, SERVO_POSITION_MAX);


    if (abs(dataExtra.acc[2])<BREAKING_ACCELERATION)
    {
      return &stateTouchdown;
    }
    return &stateBreaking;
}


