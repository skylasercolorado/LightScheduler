#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        virtual LightSchedulerTime &GetTime();
        virtual void SetPeriodicAlarm(int periodInSeconds);
        // The actual mock starts from hereon forward
        static void setMinute(int minute);
        static void setDay(int day);
        static void reset();
        static int getAlarmPeriod();

    private:
        static LightSchedulerTime time;
        static int periodInSeconds;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
