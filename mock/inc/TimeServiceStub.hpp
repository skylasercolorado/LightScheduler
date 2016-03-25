#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        virtual LightSchedulerTime &GetTime();
        virtual void SetAlarmPeriod(uint periodInSeconds);
        // The actual mock starts from hereon forward
        static void SetMinute(int minute);
        static void SetDay(int day);
        static void Reset();
        static int GetAlarmPeriod();

    private:
        static LightSchedulerTime time_;
        static int periodInSeconds_;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
