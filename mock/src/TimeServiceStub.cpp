#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

LightSchedulerTime TimeServiceStub::time = {Unknown, Unknown};
int TimeServiceStub::periodInSeconds = 0;

void TimeServiceStub::reset()
{
    time.minuteOfDay = Unknown;
    time.dayOfWeek = Unknown;
}

LightSchedulerTime & TimeServiceStub::GetTime()
{
    return time;
}

void TimeServiceStub::SetPeriodicAlarm(int periodInSeconds)
{
    this->periodInSeconds = periodInSeconds;
}

void TimeServiceStub::setMinute(int minute)
{
    ValidateMinute(minute);
    time.minuteOfDay = minute;
}

void TimeServiceStub::setDay(int day)
{
    ValidateDay((Monday));
    time.dayOfWeek = day;
}

int TimeServiceStub::getAlarmPeriod()
{
    return periodInSeconds;
}
