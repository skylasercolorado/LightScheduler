#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"
#include "Subject.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        virtual LightSchedulerTime &GetTime();
        virtual void SetAlarmPeriod(uint periodInSeconds);
        // The actual mock starts from hereon forward
        void SetMinute(uint minute);
        void SetDay(Day day);
        void Reset();
        uint GetAlarmPeriod();
        ObserverHandle<TimeServiceEvents> RegisterForTimeServiceEvent(TimeServiceEvents event,
                                                                              uint alarmPeriod,
                                                                              SubjectType::EventHandler notificationHandler);
        bool UnregisterForTimeServiceEvent(ObserverHandle<TimeServiceEvents> handle);
        void NotifyObservers(TimeServiceEvents event);
        bool FindObserver(ObserverHandle<TimeServiceEvents> handle);

    private:
        LightSchedulerTime time_;
        uint periodInSeconds_;
        SubjectType events_;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
