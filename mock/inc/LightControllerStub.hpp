#ifndef CAMAX_LIGHTCONTROLLERSTUB_HPP
#define CAMAX_LIGHTCONTROLLERSTUB_HPP

#include "ILightController.hpp"

namespace Camax
{
    class LightControllerStub : public ILightController
    {
    public:
        void TurnOn(int id);
        void TurnOff(int id);
        // The actual mock starts from hereon forward
        static int GetLastId();
        static int GetLastState();
        static void Reset();

    private:
        static int lastId_;
        static int lastState_;
    };
}

#endif // CAMAX_LIGHTCONTROLLERSTUB_HPP
