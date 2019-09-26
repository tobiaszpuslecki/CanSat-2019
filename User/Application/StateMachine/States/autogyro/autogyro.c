#include "../states.h"
#include "../../../Includes/cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateAutogyro;


int pos = SERVO_POSITION_MIN;

bool STATE_AUTOGYRO_SAVED=false;
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateAutogyroFunction(stateInput_t input, stateOutput_t* output)
{
	if (!STATE_AUTOGYRO_SAVED)
	{
		lastStateFileWrite(AUTOGYRO_CHAR);
		data.software_state = AUTOGYRO_CHAR-'0';
		STATE_AUTOGYRO_SAVED = true;
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (5) Autogyro saved!");
	}



	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET);

//



		int currentAngle = 1;


	if(HAL_GetTick() % 2000 < 1000 )
	{
		servo_set_position(servo2, SERVO_POSITION_MAX/currentAngle);
		if(currentAngle>5) currentAngle=1;
	}





//

    if (data.altitude <= 150)
    {
      return &stateBreaking;
    }
    return &stateAutogyro;
}

