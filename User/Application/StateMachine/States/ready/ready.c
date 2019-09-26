#include "../states.h"
#include "cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateReady;


bool STATE_READY_SAVED = false;
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateReadyFunction(stateInput_t input, stateOutput_t* output)
{
	if (!STATE_READY_SAVED)
	{
	  lastStateFileWrite(READY_CHAR);
	  data.software_state = READY_CHAR-'0';
	  STATE_READY_SAVED = true;
	  log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (2) Ready saved!");
	}

	if(abs(dataExtra.acc[2]) > LAUNCH_ACCELERATION)
	{
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "LAUNCH DETECTED!");

		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Ready successed!");
		return &stateFlightup;
	}
	return &stateReady;
}

