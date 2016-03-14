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

void ITimeService::validateDay(Day day)
{
    if (day < Monday || day > Everyday)
    {
        cout << "\n validateDay() exception, day: " << day;
        throw range_error ("day must be between Monday and Everyday");
    }
}

void ITimeService::validateDayMinute(int day, int minute)
{
    validateDay((Monday));
    validateMinute(minute);
}

void ITimeService::RegisterEventObserver(TimeServiceEvents event,  std::function<void(ITimeService::TimeServiceEvents)> fn)
{
    events.registerObserver(event, fn);
}

void ITimeService::NotifyObservers(TimeServiceEvents event)
{
    events.notify(event);
}

