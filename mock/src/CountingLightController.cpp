//
// Created by duncan on 4/22/16.
//

#include "CountingLightController.hpp"

using namespace Camax;

void CountingLightController::TurnOn(uint id)
{
    callCounter++;
}

void CountingLightController::TurnOff(uint id)
{
    callCounter++;
}

uint CountingLightController::getCallCounter()
{
    return callCounter;
}
