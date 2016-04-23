//
// Created by duncan on 4/22/16.
//

#ifndef LIGHTSCHEDULER_COUNTINGLIGHTCONTROLLER_HPP
#define LIGHTSCHEDULER_COUNTINGLIGHTCONTROLLER_HPP

#include <ILightController.hpp>

namespace Camax
{
    class CountingLightController : public ILightController
    {
    public:
        CountingLightController() : callCounter(0) {}
        virtual void TurnOn(uint id);
        virtual void TurnOff(uint id);
        uint getCallCounter();

    private:
        uint callCounter;
    };
}
#endif //LIGHTSCHEDULER_COUNTINGLIGHTCONTROLLER_HPP
