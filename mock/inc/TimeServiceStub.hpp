#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"
#include "Subject.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        TimeServiceStub()
        {
            std::cout << "\n TimeServiceStub object created \n";
        }
        virtual LightSchedulerTime &GetTime();
        virtual void SetAlarmPeriod(uint periodInSeconds);
        // The actual mock starts from hereon forward
        static void SetMinute(int minute);
        static void SetDay(int day);
        static void Reset();
        static int GetAlarmPeriod();
        //TODO: To be called from LightScheduler()
        ObserverHandle<TimeServiceEvents> RegisterForTimeServiceEvent(TimeServiceEvents event,
                                                                              uint alarmPeriod,
                                                                              SubjectType::EventHandler notificationHandler);
        //TODO: To be called from ~LightScheduler(), through helper safe-guarded method
        bool UnregisterForTimeServiceEvent(ObserverHandle<TimeServiceEvents> handle);
        void NotifyObservers(TimeServiceEvents event);
        bool FindObserver(ObserverHandle<TimeServiceEvents> handle);

    private:
        static LightSchedulerTime time_;
        static int periodInSeconds_;
        SubjectType events_;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
