#include "ITimeService.hpp"
#include <stdexcept>
using namespace Camax;

void ITimeService::validateMinute(int minute)
{
    const int hoursInDay = 24;
    const int minutesInHour = 60;

    if (minute < 0 || minute > ((hoursInDay * minutesInHour) - 1))
    {
        cout << "\n validateMinute() exception, minute: " << minute;
        throw range_error ("minutes must be between 0 and 1439");
    }
}

void ITimeService::validateDay(int day)
{
    if (day < Monday || day > Everyday)
    {
        cout << "\n validateDay() exception, day: " << day;
        throw range_error ("day must be between Monday and Everyday");
    }
}

void ITimeService::validateDayMinute(int day, int minute)
{
    validateDay(day);
    validateMinute(minute);
}	
