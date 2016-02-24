#ifndef CAMAX_LIGHTSCHEDULER_HPP
#define CAMAX_LIGHTSCHEDULER_HPP

#include "ITimeService.hpp"
#include "ILightController.hpp"
#include <vector>

using namespace std;

namespace Camax
{
    struct ScheduledLightEvent
    {
        int id;
        TimeStatus day;
        int minuteOfDay;
        LightStatus lightStatus;
    
        ScheduledLightEvent(int _id, TimeStatus _day, int _minuteOfDay, LightStatus _lightStatus) :
                id(_id), day(_day), minuteOfDay(_minuteOfDay), lightStatus(_lightStatus)  {}
    };
  
    class LightScheduler
    {
    public:
        LightScheduler(ITimeService &_timeService, ILightController &_lightController) :
            timeService(_timeService), lightController(_lightController) {}
        void ScheduleTurnOn(int id, TimeStatus day, int minute);
        void ScheduleTurnOff(int id, TimeStatus day, int minute);
        void RemoveSchedule();
        void WakeUp();
    
    private:
        vector<ScheduledLightEvent> scheduledLightEvents;
        ITimeService &timeService;
        ILightController &lightController;
        void validateMinute(int minute);
        void validateDay(int day);
        void validateDayMinute(int day, int minute);
    };
}

#endif // CAMAX_LIGHTSCHEDULER_HPP
