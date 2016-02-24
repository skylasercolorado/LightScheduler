#include "LightController.hpp"

using Camax::LightController;

int LightController::lastId = LightIdUnknown;
int LightController::lastState = LightStateUnknown;

void LightController::reset()
{
  LightController::lastId = LightIdUnknown;
  LightController::lastState = LightStateUnknown;
}

void LightController::turnOn(int id)
{
  lastId = id;
  lastState = LightStateOn;
}

void LightController::turnOff(int id)
{
  lastId = id;
  lastState = LightStateOff;
}

int LightController::getLastId()
{
  return lastId;
}

int LightController::getLastState()
{
  return lastState;
}
