#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

Time TimeServiceStub::time = {TimeUnknown, TimeUnknown};

void TimeServiceStub::validateMinute(int minute)
{
    const int hoursInDay = 24;
    const int minutesInHour = 60;

    if (minute < 0 || minute > ((hoursInDay * minutesInHour) - 1))
    {
        cout << "\n validateMinute() exception, minute: " << minute;
        throw range_error ("minutes must be between 0 and 1439");
    }
}

void TimeServiceStub::validateDay(int day)
{
    if (day < Monday || day > Everyday)
    {
        cout << "\n validateDay() exception, day: " << day;
        throw range_error ("day must be between Monday and Everyday");
    }
}

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
