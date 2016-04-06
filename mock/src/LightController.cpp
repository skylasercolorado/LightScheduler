#include "LightController.hpp"

using Camax::LightController;
using namespace Camax;

uint LightController::lastId_ = LightIdUnknown;
LightStatus LightController::lastState_ = LightStateUnknown;
LightController::LightControllerStore LightController::lightControllerStore_;

void LightController::Reset()
{
    lastId_ = LightIdUnknown;
    lastState_ = LightStateUnknown;
    lightControllerStore_.clear();
}

void LightController::TurnOn(uint id)
{
    lastId_ = id;
    lastState_ = LightStateOn;

    storeLightState(lastId_, lastState_);
}

void LightController::TurnOff(uint id)
{
    lastId_ = id;
    lastState_ = LightStateOff;

    storeLightState(lastId_, lastState_);
}

void LightController::storeLightState(uint id, LightStatus state)
{
    LightControllerStore::iterator it;

    it = lightControllerStore_.find(id);
    if(it != lightControllerStore_.end())
        lightControllerStore_.at(id) = state;
    else
        lightControllerStore_.insert({id, state});
}

LightStatus LightController::GetLightState(uint id)
{
    LightControllerStore::iterator it;

    it = lightControllerStore_.find(id);
    if(it != lightControllerStore_.end())
       return it->second;
    else
       return LightStateUnknown;
}

uint LightController::GetLastId()
{
    return lastId_;
}

LightStatus LightController::GetLastState()
{
    return lastState_;
}
