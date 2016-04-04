#ifndef CAMAX_ILIGHTCONTROLLER_HPP
#define CAMAX_ILIGHTCONTROLLER_HPP

#include <iostream>

namespace Camax
{
    enum LightStatus
    {
        LightIdUnknown    = -1,
        LightStateUnknown = -2,
        LightStateOn      = 1,
        LightStateOff     = 2
    };

    class ILightController
    {
    public:
        virtual void TurnOn(uint id) = 0;
        virtual void TurnOff(uint id) = 0;
    };
}

#endif // CAMAX_ILIGHTCONTROLLER_HPP
