/**
    \Brief:     StateMachine_t struct implementation

    \File:      StateMachine.c

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#include "StateMachine.h"

StateMachineReturn_t stateMachine_init(StateMachine_t* stateMachine, statesInitFunction statesInitFunction, State_t* const* statesList, uint16_t numStates)
{
    if(stateMachine == NULL)
    {
        return STATEMACHINE_RETURN_INCORRECT_HANDLER;
    }

    stateMachine->_statesList = statesList;
    stateMachine->_numStates = numStates;
    stateMachine->_statesInited = statesInitFunction();
    stateMachine->_ticks = 0;

    return stateMachine_isReady(stateMachine);
}

StateMachineReturn_t stateMachine_run(StateMachine_t* stateMachine, State_t* stateStart, stateInput_t input)
{
    if (stateMachine_isReady(stateMachine))
    {
        return stateMachine_isReady(stateMachine);
    }

    stateOutput_t output;

    for(int q = 0; q < stateMachine->_numStates; ++q)
    {
        if(stateMachine->_statesList[q] == stateStart)
        {
            stateMachine->_stateCurrent = stateStart;
            stateStart = state_execute(stateStart, input, &output);
            stateMachine->_stateCurrent = NULL;

            input = output;
            
            q = -1;
        }
    }

    return STATEMACHINE_RETURN_OK;
}

void stateMachine_updateTicksIRQ(StateMachine_t* stateMachine)
{
    if(stateMachine_isReady(stateMachine))
    {
        return;
    }

    if(stateMachine->_stateCurrent == NULL)
    {
        ++stateMachine->_ticks;
    }
    else
    {
        state_incTicks(stateMachine->_stateCurrent);
    }
}

StateMachineReturn_t stateMachine_isReady(StateMachine_t* stateMachine)
{
    if (stateMachine == NULL)
    {
        return STATEMACHINE_RETURN_INCORRECT_HANDLER;
    }

    if(stateMachine->_statesInited != 0 || stateMachine->_statesList == NULL || stateMachine->_numStates == 0)
    {
        return STATEMACHINE_RETURN_UNINITIALIZED;
    }

    return STATEMACHINE_RETURN_OK;
}