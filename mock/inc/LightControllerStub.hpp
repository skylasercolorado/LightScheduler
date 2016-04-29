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
        virtual void Brighten(uint id);
        virtual void Dim(uint id);
        virtual void Strobe(uint id);
        // The actual mock starts from hereon forward
        uint GetLastId();
        LightStatus GetLastState();
        void Reset();
        LightStatus GetLightState(uint id);

    private:
        uint lastId_;
        LightStatus lastState_;
        LightControllerStore lightControllerStore_;
        void storeLightState(uint id, LightStatus state);

        void setLightState(uint id, LightStatus state);
    };
}

#endif // CAMAX_LIGHTCONTROLLERSTUB_HPP
