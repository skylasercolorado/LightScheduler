#ifndef CAMAX_ILIGHTCONTROLLER_HPP
#define CAMAX_ILIGHTCONTROLLER_HPP

#include <iostream>

namespace Camax
{
    enum LightStatus
    {
        LightIdUnknown          = -1,
        LightStateUnknown       = -2,
        LightStateOn            = 1,
        LightStateOff           = 2,
        LightStateBrightened    = 3,
        LightStateDimmed        = 4,
        LightStateStrobed       = 5
    };

    class ILightController
    {
    public:
        virtual void TurnOn(uint id)    = 0;
        virtual void TurnOff(uint id)   = 0;
        virtual void Brighten(uint id)  {};
        virtual void Dim(uint id)       {};
        virtual void Strobe(uint id)    {};
    };
}

#endif // CAMAX_ILIGHTCONTROLLER_HPP
