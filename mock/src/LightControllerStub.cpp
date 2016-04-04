#include "LightControllerStub.hpp"

using Camax::LightControllerStub;
using namespace Camax;

uint LightControllerStub::lastId_ = LightIdUnknown;
LightStatus LightControllerStub::lastState_ = LightStateUnknown;

void LightControllerStub::Reset()
{
    lastId_ = LightIdUnknown;
    lastState_ = LightStateUnknown;
}

void LightControllerStub::TurnOn(uint id)
{
    lastId_ = id;
    lastState_ = LightStateOn;

    storeLightState(lastId_, lastState_);
}

void LightControllerStub::TurnOff(uint id)
{
    lastId_ = id;
    lastState_ = LightStateOff;

    storeLightState(lastId_, lastState_);
}

void LightControllerStub::storeLightState(uint id, LightStatus state)
{
    LightControllerStore::iterator it;

    it = lightControllerStore_.find(id);
    if(it != lightControllerStore_.end())
        lightControllerStore_.at(id) = state;
    else
        lightControllerStore_.insert({id, state});
}

LightStatus LightControllerStub::GetLightState(uint id)
{
    LightControllerStore::iterator it;

    it = lightControllerStore_.find(id);
    if(it != lightControllerStore_.end())
       return it->second;
    else
       return LightStateUnknown;
}

uint LightControllerStub::GetLastId()
{
    return lastId_;
}

LightStatus LightControllerStub::GetLastState()
{
    return lastState_;
}
