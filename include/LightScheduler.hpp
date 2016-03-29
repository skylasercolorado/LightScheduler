#ifndef CAMAX_LIGHTSCHEDULER_HPP
#define CAMAX_LIGHTSCHEDULER_HPP

#include "ITimeService.hpp"
#include "ILightController.hpp"
#include <vector>

using namespace std;
using namespace std::placeholders;

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
                timeService_(_timeService), lightController_(_lightController)
        {
            notificationHandler_ = std::bind(&LightScheduler::NotificationHandler, this, _1);
        }
        //TODO: Add destructor and test with artificial block. Then, write helper destroy() that keeps tab if it already
        //      has been called (no need to use artificial block).
        void ScheduleTurnOn(int id, Day day, int minute);
        void ScheduleTurnOff(int id, Day day, int minute);
        void RemoveSchedule();
        void WakeUp();
        //TODO: Should be private.
        void NotificationHandler(ITimeService::TimeServiceEvents event);
        //TODO: This shouldn't be here. The mock should be called instead. These two below.
        //      Called from LightScheduler()
        ObserverHandle<ITimeService::TimeServiceEvents> RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents event, uint alarmPeriod);
        //      Called from ~LightScheduler(), through helper safe-guarded method
        bool UnregisterForTimeServiceEvent(ObserverHandle<ITimeService::TimeServiceEvents> handle);

    private:
        vector<ScheduledLightEvent> scheduledLightEvents_;
        ITimeService &timeService_;
        ILightController &lightController_;
        bool doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator &event);
        void operateLight(vector<Camax::ScheduledLightEvent>::iterator &event);
        Subject<ITimeService::TimeServiceEvents>::EventHandler notificationHandler_;
    };
}

#endif // CAMAX_LIGHTSCHEDULER_HPP
