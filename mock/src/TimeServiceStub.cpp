#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

Time TimeServiceStub::time = {Unknown, Unknown};
int TimeServiceStub::periodInSeconds = 0;

void TimeServiceStub::reset()
{
    time.minuteOfDay = Unknown;
    time.dayOfWeek = Unknown;
}

Time& TimeServiceStub::getTime()
{
    return time;
}

void TimeServiceStub::setPeriodicAlarm(int periodInSeconds)
{
    this->periodInSeconds = periodInSeconds;
}

void TimeServiceStub::setMinute(int minute)
{
    validateMinute(minute);
    time.minuteOfDay = minute;
}

void TimeServiceStub::setDay(int day)
{
    validateDay((Monday));
    time.dayOfWeek = day;
}

int TimeServiceStub::getAlarmPeriod()
{
    return periodInSeconds;
}
