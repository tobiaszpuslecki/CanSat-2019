/**
    \Brief:     All of states initialise

    \File:      States.c

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#include "states.h"

State_t* const statesList[] = {&statePrelaunch, 
                         &stateReady, 
                         &stateFlightup, 
                         &stateContainer, 
                         &stateAutogyro,
                         &stateBreaking,
                         &stateTouchdown };

const uint16_t numStates = sizeof(statesList) / sizeof(State_t*);

State_t* statePrelaunchFunction(stateInput_t input, stateOutput_t* output);
State_t* stateReadyFunction(stateInput_t input, stateOutput_t* output);
State_t* stateFlightupFunction(stateInput_t input, stateOutput_t* output);
State_t* stateContainerFunction(stateInput_t input, stateOutput_t* output);
State_t* stateAutogyroFunction(stateInput_t input, stateOutput_t* output);
State_t* stateBreakingFunction(stateInput_t input, stateOutput_t* output);
State_t* stateTouchdownFunction(stateInput_t input, stateOutput_t* output);

StateReturn_t statesInit()
{
    uint16_t fail = 0;

    fail += state_init(&statePrelaunch, statePrelaunchFunction);
    fail += state_init(&stateReady, stateReadyFunction);
    fail += state_init(&stateFlightup, stateFlightupFunction);
    fail += state_init(&stateContainer, stateContainerFunction);
    fail += state_init(&stateAutogyro, stateAutogyroFunction);
    fail += state_init(&stateBreaking, stateBreakingFunction);
    fail += state_init(&stateTouchdown, stateTouchdownFunction);

    if(fail)
    {
        return STATE_RETURN_FAIL;
    }

    return STATE_RETURN_OK;
}