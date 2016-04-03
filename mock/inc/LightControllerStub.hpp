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
        void TurnOn(int id);
        void TurnOff(int id);
        // The actual mock starts from hereon forward
        static int GetLastId();
        static int GetLastState();
        static void Reset();
        LightStatus GetLightState(uint id);

    private:
        static int lastId_;
        static LightStatus lastState_;
        LightControllerStore lightControllerStore_;
        LightControllerStore::iterator it;
        void storeLightState(uint id, LightStatus state);
    };
}

#endif // CAMAX_LIGHTCONTROLLERSTUB_HPP
