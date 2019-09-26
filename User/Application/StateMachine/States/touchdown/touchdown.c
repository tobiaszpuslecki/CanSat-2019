#include "../states.h"
#include "cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateTouchdown;

bool STATE_TOUCHDOWN_SAVED=false;
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateTouchdownFunction(stateInput_t input, stateOutput_t* output)
{

    if (!STATE_TOUCHDOWN_SAVED)
	{
	  lastStateFileWrite(TOUCHDOWN_CHAR);
	  data.software_state = TOUCHDOWN_CHAR-'0';
	  STATE_TOUCHDOWN_SAVED = true;
	  closeLoggerFile();
	  log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (7) Touchdown saved!");
	}

    HAL_TIM_Base_Stop_IT(&htim4);
    HAL_UART_Transmit(&huart2, "S", 1, HAL_MAX_DELAY);

	//Mission complete
	return &stateTouchdown;
}

