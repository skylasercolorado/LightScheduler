#include "LightControllerStub.hpp"

using Camax::LightControllerStub;

int LightControllerStub::lastId = LightIdUnknown;
int LightControllerStub::lastState = LightStateUnknown;

void LightControllerStub::reset()
{
    lastId = LightIdUnknown;
    lastState = LightStateUnknown;
}

void LightControllerStub::turnOn(int id)
{
    lastId = id;
    lastState = LightStateOn;
}

void LightControllerStub::turnOff(int id)
{
    lastId = id;
    lastState = LightStateOff;
}

int LightControllerStub::getLastId()
{
    return lastId;
}

int LightControllerStub::getLastState()
{
    return lastState;
}
