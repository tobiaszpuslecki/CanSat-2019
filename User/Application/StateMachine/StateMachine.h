/**
    \Brief:     StateMachine_t struct header

    \File:      StateMachine.h

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

#include "State/State.h"

/**
    States init function pointer
*/
typedef StateReturn_t(*statesInitFunction)();

/**
    Possible outputs from State Machine's functions
*/
typedef enum StateMachineReturn_t
{
    STATEMACHINE_RETURN_OK = 0,
    STATEMACHINE_RETURN_FAIL,
    STATEMACHINE_RETURN_INCORRECT_HANDLER,
    STATEMACHINE_RETURN_UNINITIALIZED,
}StateMachineReturn_t;

/**
    \Breif: StateMachine_t struct header

    \Note: Do not modify the structure pools manually!
*/
typedef struct StateMachine_t
{
//      PRIVATE     //
    /* List of address used states */
    State_t* const* _statesList;

    /* Size of list */
    uint16_t _numStates;

    /* State address of current state */
    State_t* _stateCurrent;

    /* Result of initialise states */
    int8_t _statesInited;

    /* _ticks is incremented every 1 [ms] if state is active */
    uint32_t _ticks;
}StateMachine_t;

/**
    \Breif: Initializes StateMachine_t struct. Should be first used function.

    \Param: [in] - StateMachine_t struct address
            [in] - States initialise function address
            [in] - States list address
            [in] - Size of states list

    \Return: StateMachineReturn_t enum
*/
StateMachineReturn_t stateMachine_init(StateMachine_t*, statesInitFunction, State_t* const*, uint16_t);

/**
    \Breif: Start the State Machine

    \Param: [in] - StateMachine_t struct address
            [in] - Start State struct address
            [in] - Input for start State

    \Return: StateMachineReturn_t enum
*/
StateMachineReturn_t stateMachine_run(StateMachine_t*, State_t*, stateInput_t);

/**
    \Breif: Increment current state's ticks

    \Param: [in] - StateMachine_t struct address

    \Note: Should be called in sysTick IRQ
*/
void stateMachine_updateTicksIRQ(StateMachine_t*);

/**
    \Breif: StateMachine_t's fuse
 
    \Param: [in] - StateMachine_t struct address

    \Return: StateMachineReturn_t enum ( 0 = OK )
*/
StateMachineReturn_t stateMachine_isReady(StateMachine_t*);

#endif