#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

LightSchedulerTime TimeServiceStub::time_ = {Unknown, Unknown};
int TimeServiceStub::periodInSeconds_ = 0;

void TimeServiceStub::Reset()
{
    time_.minuteOfDay = Unknown;
    time_.dayOfWeek = Unknown;
}

LightSchedulerTime & TimeServiceStub::GetTime()
{
    return time_;
}

void TimeServiceStub::SetAlarmPeriod(int periodInSeconds)
{
    this->periodInSeconds_ = periodInSeconds;
}

void TimeServiceStub::SetMinute(int minute)
{
    ValidateMinute(minute);
    time_.minuteOfDay = minute;
}

void TimeServiceStub::SetDay(int day)
{
    ValidateDay((Monday));
    time_.dayOfWeek = day;
}

int TimeServiceStub::GetAlarmPeriod()
{
    return periodInSeconds_;
}
