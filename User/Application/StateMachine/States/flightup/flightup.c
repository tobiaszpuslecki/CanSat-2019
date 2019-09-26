#include "../states.h"
#include "cansat_includes.h"

/*====================================*/
/*            DECLARATIONS            */
/*====================================*/
State_t stateFlightup;

bool STATE_FLIGHTUP_SAVED = false;
bool HIGHEST_POINT = false;

int safetyApogeeCountdownStart = 0;
int apogeeCountdownStart = 0;
int pendingApogee = 0;
int apogeeCountdownCheck = 0;


bool checkApogeeCountdowns()
{
  // If the apogee countdown is finished, fire it
  if(apogeeCountdownStart > 0 && abs(apogeeCountdownStart - HAL_GetTick() ) >= APOGEE_COUNTDOWN) {
    return true;
  }

  // If the safety apogee countdown is finished, fire it
  if(safetyApogeeCountdownStart > 0 && abs(safetyApogeeCountdownStart - HAL_GetTick() ) >= SAFETY_APOGEE_COUNTDOWN) {
    return true;
  }

  return false;
}
/*====================================*/
/*           GLOBAL FUNCTION          */
/*====================================*/
State_t* stateFlightupFunction(stateInput_t input, stateOutput_t* output)
{

	if (!STATE_FLIGHTUP_SAVED)
	{
	  lastStateFileWrite(FLIGHTUP_CHAR);
	  data.software_state = FLIGHTUP_CHAR-'0';
	  STATE_FLIGHTUP_SAVED = true;
	  log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "State (3) Flightup saved!");
	}

	if(apogeeCountdownStart == 0) apogeeCountdownStart = HAL_GetTick();


	// If apogee is pending, as soon as the altitude decreases, fire it
	if(pendingApogee)
	{
	  if(dataExtra.previousAltitude - data.altitude > APOGEE_ALTITUDE_DELTA)
	  {
		return &stateContainer;
	  }
	}

	// If the apogee countdown is finished, fire it
	if(checkApogeeCountdowns())
	{
	  return &stateContainer;
	}

	// Anything less than the ideal acceleration means we're basically at apogee, but should start paying attention to altitude to get as close as possible
	if(abs(dataExtra.acc[2]) < APOGEE_IDEAL)
	{
	  pendingApogee = 1;
	  // Only start the countdown if it's not already started
	  if(apogeeCountdownStart == 0) apogeeCountdownStart = HAL_GetTick();
	}

	// Anything less than okay acceleration is /probably/ apogee, but wait to see if we
	// can get closer and if not, the timer will expire causing an apogee event
	if(abs(dataExtra.acc[2]) < APOGEE_OKAY)
	{
	  // Only start the countdown if it's not already started
	  if(safetyApogeeCountdownStart == 0) safetyApogeeCountdownStart = HAL_GetTick();
	}

	// If the acceleration is back to 1 then we're falling but without a drogue chute (uh oh)
	/*if(dataExtra.acc[2] > ACCELERATION_ONE)
	{
		 log_data(LOG_TYPE_INFO, LOG_DEVICE_SYSTEM, "ONE!");
	  return &stateContainer;
	}*/


	return &stateFlightup;
}



