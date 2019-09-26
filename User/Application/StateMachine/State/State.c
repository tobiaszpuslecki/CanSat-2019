/**
    \Brief:     State_t struct implementation

    \File:      State.c

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#include "State.h"

StateReturn_t state_init(State_t* state, stateFunctionAddress_t stateFunctionAddres)
{
    if(state == NULL)
    {
        return STATE_RETURN_INCORRECT_HANDLER;
    }

    state->_stateFunctionAddress = stateFunctionAddres;
    state->_ticks = 0;

    return state_isReady(state);
}

State_t* state_execute(State_t* state, stateInput_t stateInput, stateOutput_t* stateOutput)
{
    if(state_isReady(state))
    {
        return STATE_EXIT;
    }

    *stateOutput = 0;

    return state->_stateFunctionAddress(stateInput, stateOutput);
}

uint32_t state_getTicks(State_t* state)
{
    if(state_isReady(state))
    {
        return 0;
    }

    return state->_ticks;
}

void state_incTicks(State_t* state)
{
    if(state_isReady(state))
    {
        return;
    }

    ++state->_ticks;
}

StateReturn_t state_isReady(State_t* state)
{
    if(state == NULL)
    {
        return STATE_RETURN_INCORRECT_HANDLER;
    }

    if(state->_stateFunctionAddress == NULL)
    {
        return STATE_RETURN_UNINITIALIZED;
    }

    return STATE_RETURN_OK;
}