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
        ILightController()
        {
            std::cout << "\n ILightController \n";
        }
        virtual void TurnOn(int id) = 0;
        virtual void TurnOff(int id) = 0;
    };
}

#endif // CAMAX_ILIGHTCONTROLLER_HPP
