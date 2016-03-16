#include "LightControllerStub.hpp"

using Camax::LightControllerStub;

int LightControllerStub::lastId_ = LightIdUnknown;
int LightControllerStub::lastState_ = LightStateUnknown;

void LightControllerStub::Reset()
{
    lastId_ = LightIdUnknown;
    lastState_ = LightStateUnknown;
}

void LightControllerStub::TurnOn(int id)
{
    lastId_ = id;
    lastState_ = LightStateOn;
}

void LightControllerStub::TurnOff(int id)
{
    lastId_ = id;
    lastState_ = LightStateOff;
}

int LightControllerStub::GetLastId()
{
    return lastId_;
}

int LightControllerStub::GetLastState()
{
    return lastState_;
}
