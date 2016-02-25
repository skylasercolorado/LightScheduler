#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

Time TimeServiceStub::time = {TimeUnknown, TimeUnknown};

void TimeServiceStub::reset()
{
    time.minuteOfDay = TimeUnknown;
    time.dayOfWeek = TimeUnknown;
}

Time& TimeServiceStub::getTime()
{
    return time;
}

void TimeServiceStub::setPeriodicAlarm()
{

}

void TimeServiceStub::setMinute(int minute)
{
    validateMinute(minute);
    time.minuteOfDay = minute;
}

void TimeServiceStub::setDay(int day)
{
    validateDay(day);
    time.dayOfWeek = day;
}
