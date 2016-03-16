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
        int id_;
        Day day_;
        int minuteOfDay_;
        LightStatus lightStatus_;
    
        ScheduledLightEvent(int id, Day day, int minuteOfDay, LightStatus lightStatus) :
                id_(id), day_(day), minuteOfDay_(minuteOfDay), lightStatus_(lightStatus)  {}
    };
  
    class LightScheduler
    {
    public:
        LightScheduler(ITimeService &_timeService, ILightController &_lightController) :
                timeService_(_timeService), lightController_(_lightController) {}
        void ScheduleTurnOn(int id, Day day, int minute);
        void ScheduleTurnOff(int id, Day day, int minute);
        void RemoveSchedule();
        void WakeUp();
        void NotificationHandler(ITimeService::TimeServiceEvents event);
        void RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents event);
    
    private:
        vector<ScheduledLightEvent> scheduledLightEvents_;
        ITimeService &timeService_;
        ILightController &lightController_;
        bool doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator event);
        void operateLight(vector<Camax::ScheduledLightEvent>::iterator event);
    };
}

#endif // CAMAX_LIGHTSCHEDULER_HPP
