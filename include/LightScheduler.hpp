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
        uint id_;
        Day day_;
        uint minuteOfDay_;
        LightStatus lightStatus_;
    
        ScheduledLightEvent(uint id, Day day, uint minuteOfDay, LightStatus lightStatus = LightStateUnknown) :
                id_(id), day_(day), minuteOfDay_(minuteOfDay), lightStatus_(lightStatus)  {}
    };
  
    class LightScheduler
    {
    public:
        LightScheduler(ITimeService &_timeService, ILightController &_lightController, uint _alarmPeriod) :
                timeService_(_timeService), lightController_(_lightController), alarmPeriod_(_alarmPeriod),
                alreadyDestroyed_(false)
        {
            observerHandle_ = timeService_.RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents::AlarmActive,
                                                                       alarmPeriod_,
                                                                       std::bind(&LightScheduler::WakeUp, this));
        }
        ~LightScheduler()
        {
            destroy();
        }
        void ScheduleTurnOn(uint id, Day day, uint minute);
        void ScheduleTurnOff(uint id, Day day, uint minute);
        void RemoveSchedule(uint id, Day day, uint minute);
        void WakeUp();
        ObserverHandle<ITimeService::TimeServiceEvents> getObserverHandle();
        void destroy();

    private:
        vector<ScheduledLightEvent> scheduledLightEvents_;
        ITimeService &timeService_;
        ILightController &lightController_;
        bool doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator &event);
        void operateLight(vector<Camax::ScheduledLightEvent>::iterator &event);
        uint alarmPeriod_;
        bool alreadyDestroyed_;
        ObserverHandle<ITimeService::TimeServiceEvents> observerHandle_;
        bool compare(ScheduledLightEvent event, ScheduledLightEvent x);
    };
}

//TODO: able to handle different light controllers
//TODO:- able to select at run time which specific controller to use.
//TODO:- new methods to support:
//TODO:- brightening()
//TODO:- dimming()
//TODO:- strobe()

#endif // CAMAX_LIGHTSCHEDULER_HPP
