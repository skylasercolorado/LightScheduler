#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"
#include "Subject.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        typedef Subject<TimeServiceEvents> SubjectType;

    public:
        virtual LightSchedulerTime &GetTime();
        virtual void SetAlarmPeriod(uint periodInSeconds);
        // The actual mock starts from hereon forward
        static void SetMinute(int minute);
        static void SetDay(int day);
        static void Reset();
        static int GetAlarmPeriod();

        // TODO: These Observer methods should be moved to the mock.
        ObserverHandle<TimeServiceEvents> RegisterObserver(TimeServiceEvents event,
                                                           SubjectType::EventHandler fn);

        void NotifyObservers(TimeServiceEvents event);

        bool UnregisterObserver(ObserverHandle<TimeServiceEvents> handle);

        bool FindObserver(ObserverHandle<ITimeService::TimeServiceEvents> handle);

        //TODO: This shouldn't be here. The mock should be called instead. These two below.
        //      Called from LightScheduler()
        ObserverHandle<TimeServiceEvents> RegisterForTimeServiceEvent(TimeServiceEvents event,
                                                                      uint alarmPeriod,
                                                                      SubjectType::EventHandler notificationHandler);

        //      Called from ~LightScheduler(), through helper safe-guarded method
        bool UnregisterForTimeServiceEvent(ObserverHandle<ITimeService::TimeServiceEvents> handle);

    private:
        static LightSchedulerTime time_;
        static int periodInSeconds_;
        // TODO: This subject object should be moved to the mock.
        SubjectType events_;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
