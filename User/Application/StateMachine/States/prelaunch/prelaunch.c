#include "../states.h"
#include "cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t statePrelaunch;

bool STATE_SAVED = false;
bool RADIO_SIGNAL_RECEIVED = false;
bool SET_REF_VAL = false;
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* statePrelaunchFunction(stateInput_t input, stateOutput_t* output)
{


    if (!STATE_SAVED)
    {
      lastStateFileWrite(PRELAUNCH_CHAR);
      data.software_state = PRELAUNCH_CHAR-'0';
      STATE_SAVED = true;
      log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State: Prelaunch");
      log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (1) Prelaunch saved!");
    }

    if (RADIO_SIGNAL_RECEIVED)
    {
		referenceFileWrite(referenceStruct);
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Reference saved!");
		log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "Prelaunch successed!");
		return &stateReady;
    }
    return &statePrelaunch;
}


