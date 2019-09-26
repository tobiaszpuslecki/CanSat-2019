/**
    \Brief:     State_t struct header

    \File:      State.h

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#include <stdint.h>

#define NULL 0
#define STATE_EXIT 0

/**
    Variable types used in states
*/
typedef int32_t stateInput_t;
typedef int32_t stateOutput_t;

/** 
    Struct of state's function
*/
struct State_t;
typedef struct State_t*(* stateFunctionAddress_t)(stateInput_t, stateOutput_t*);

/**
    Possible outputs from state's functions
*/
typedef enum StateReturn_t
{
    STATE_RETURN_OK = 0,
    STATE_RETURN_FAIL,
    STATE_RETURN_INCORRECT_HANDLER,
    STATE_RETURN_UNINITIALIZED,
}StateReturn_t;

/**
    \Brief: State_t struct header

    \Note: Do not modify the structure pools manually!
*/
typedef struct State_t
{
//      PRIVATE     //
    /* State_t's function address */
    stateFunctionAddress_t _stateFunctionAddress;
   
    /* _ticks is incremented every 1 [ms] if state is active */
    uint32_t _ticks;
}State_t;


/**
    \Breif: Initializes State_t struct. Should be first used function.

    \Param: [in] - State_t struct address
            [in] - State_t's function address

    \Return: StateReturn_t enum
*/
StateReturn_t state_init(State_t*, stateFunctionAddress_t);

/**
    \Breif: Calls State_t's function

    \Param: [in] - State_t struct address
            [in] - Variable received by State_t's function
            [out] - Variable transferred from State_t's function to next State_t's function

    \Return: State_t struct address which should be executed next
*/
State_t* state_execute(State_t*, stateInput_t, stateOutput_t*);

/**
    \Breif: States's _ticks variable getter. The variable unit is [ms]

    \Param: [in] - State_t struct address

    \Return: _ticks
*/
uint32_t state_getTicks(State_t*);

/**
    \Brief: Increment State_t's ticks

    \Param: [in] - State_t struct address
*/
void state_incTicks(State_t*);

/**
    \Breif: State_t's fuse

    \Param: [in] - State_t struct address

    \Return: StateReturn_t enum ( 0 = OK )

*/
StateReturn_t state_isReady(State_t*);

#endif
