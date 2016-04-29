#include "LightControllerStub.hpp"

using Camax::LightControllerStub;
using namespace Camax;

void LightControllerStub::setLightState(uint id, LightStatus state)
{
    lastId_ = id;
    lastState_ = state;

    storeLightState(lastId_, lastState_);
}

void LightControllerStub::Reset()
{
    lastId_ = LightIdUnknown;
    lastState_ = LightStateUnknown;
}

void LightControllerStub::TurnOn(uint id)
{
    setLightState(id, LightStateOn);
}

void LightControllerStub::TurnOff(uint id)
{
    setLightState(id, LightStateOff);
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

void LightControllerStub::Brighten(uint id)
{

}

void LightControllerStub::Dim(uint id)
{

}

void LightControllerStub::Strobe(uint id)
{

}
