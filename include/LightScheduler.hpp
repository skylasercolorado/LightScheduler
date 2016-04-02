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
        LightScheduler(ITimeService &_timeService, ILightController &_lightController, uint _alarmPeriod) :
                timeService_(_timeService), lightController_(_lightController), alarmPeriod_(_alarmPeriod),
                alreadyDestroyed_(false)
        {
            //TODO: Cannot call these virtual inherited methods here, because at this point, the referenced
            // objects are not yet created. This is causing the segmentation fault. Solution: Make a helper method
            // and call it from the application level or create these independent objects before LightScheduler.
            observerHandle_ = timeService_.RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents::AlarmActive,
                                                                       alarmPeriod_,
                                                                       std::bind(&LightScheduler::WakeUp, this));
        }
        //TODO: Add destructor and test with artificial block. Then, write helper destroy() that keeps tab if it already
        //      has been called (no need to use artificial block).
        ~LightScheduler()
        {
            if(!alreadyDestroyed_)
                destroy();
        }
        void ScheduleTurnOn(int id, Day day, int minute);
        void ScheduleTurnOff(int id, Day day, int minute);
        void RemoveSchedule();
        void WakeUp();
        ObserverHandle<ITimeService::TimeServiceEvents> getObserverHandle();

    private:
        vector<ScheduledLightEvent> scheduledLightEvents_;
        ITimeService &timeService_;
        ILightController &lightController_;
        bool doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator &event);
        void operateLight(vector<Camax::ScheduledLightEvent>::iterator &event);
        bool alreadyDestroyed_;
        void destroy();
        ObserverHandle<ITimeService::TimeServiceEvents> observerHandle_;
        uint alarmPeriod_;
    };
}

#endif // CAMAX_LIGHTSCHEDULER_HPP
