/**
    \Brief:     All of states headers

    \File:      States.h

    \Author:    Copyright (C) 2019-2020 Mateusz Adamczyk

    \Licence:   Mozilla Public License ("MPL")
*/
#ifndef INCLUDE_STATES_H
#define INCLUDE_STATES_H

#include "../State/State.h"

extern State_t statePrelaunch;
extern State_t stateReady;
extern State_t stateFlightup;
extern State_t stateContainer;
extern State_t stateAutogyro;
extern State_t stateBreaking;
extern State_t stateTouchdown;

extern State_t* const statesList[];
extern const uint16_t numStates;

/**
    \Brief: Initialise all states

    \Return: StateReturn_t enum ( 0 = OK )
*/
StateReturn_t statesInit();

#endif