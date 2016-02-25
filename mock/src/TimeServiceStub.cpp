#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

Time TimeServiceStub::time = {Unknown, Unknown};

void TimeServiceStub::reset()
{
    time.minuteOfDay = Unknown;
    time.dayOfWeek = Unknown;
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
    validateDay((Monday));
    time.dayOfWeek = day;
}
