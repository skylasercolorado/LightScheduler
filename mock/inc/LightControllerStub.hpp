#ifndef CAMAX_LIGHTCONTROLLERSTUB_HPP
#define CAMAX_LIGHTCONTROLLERSTUB_HPP

#include <map>
#include "ILightController.hpp"

namespace Camax
{
    class LightControllerStub : public ILightController
    {
    public:
        typedef std::map<uint, LightStatus> LightControllerStore;
        void TurnOn(uint id);
        void TurnOff(uint id);
        // The actual mock starts from hereon forward
        uint GetLastId();
        LightStatus GetLastState();
        void Reset();
        LightStatus GetLightState(uint id);

    private:
//        static uint lastId_;
        uint lastId_;
//        static LightStatus lastState_;
        LightStatus lastState_;
        LightControllerStore lightControllerStore_;
        void storeLightState(uint id, LightStatus state);
    };
}

#endif // CAMAX_LIGHTCONTROLLERSTUB_HPP
