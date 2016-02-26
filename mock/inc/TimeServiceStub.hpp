#ifndef CAMAX_TIMESERVICESTUB_HPP
#define CAMAX_TIMESERVICESTUB_HPP

#include "ITimeService.hpp"

namespace Camax
{
    class TimeServiceStub : public ITimeService
    {
    public:
        Time& getTime();
        void setPeriodicAlarm(int periodInSeconds, TimeServiceCallBack callBack);
        // The actual mock starts from hereon forward
        static void setMinute(int minute);
        static void setDay(int day);
        static void reset();
        static TimeServiceCallBack getAlarmCallBack();
        static int getAlarmPeriod();

    private:
        static Time time;
        static int periodInSeconds;
        static TimeServiceCallBack callBackFunction;
    };
}

#endif // CAMAX_TIMESERVICESTUB_HPP
