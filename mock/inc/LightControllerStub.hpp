#ifndef CAMAX_LIGHTCONTROLLERSTUB_HPP
#define CAMAX_LIGHTCONTROLLERSTUB_HPP

#include "ILightController.hpp"

namespace Camax
{
    class LightControllerStub : public ILightController
    {
    public:
        void turnOn(int id);
        void turnOff(int id);
        // The actual mock starts from hereon forward
        static int getLastId();
        static int getLastState();
        static void reset();

    private:
        static int lastId;
        static int lastState;
    };
}

#endif // CAMAX_LIGHTCONTROLLERSTUB_HPP
