#include "ITimeService.hpp"
#include "TimeServiceStub.hpp"
#include <stdexcept>

using namespace Camax;

void ITimeService::ValidateMinute(int minute)
{
    const int hoursInDay = 24;
    const int minutesInHour = 60;

    if (minute < 0 || minute > ((hoursInDay * minutesInHour) - 1))
    {
        cout << "\n ValidateMinute() exception, minute: " << minute;
        throw range_error ("minutes must be between 0 and 1439");
    }
}

void ITimeService::ValidateDay(Day day)
{
    if (day < Monday || day > Everyday)
    {
        cout << "\n ValidateDay() exception, day: " << day;
        throw range_error ("day must be between Monday and Everyday");
    }
}

void ITimeService::ValidateDayMinute(int day, int minute)
{
    ValidateDay((Monday));
    ValidateMinute(minute);
}

