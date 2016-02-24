#include "LightScheduler.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace Camax;
using namespace std;

void LightScheduler::validateMinute(int minute)
{
    const int hoursInDay = 24;
    const int minutesInHour = 60;

    if (minute < 0 || minute > ((hoursInDay * minutesInHour) - 1))
    {
        cout << "\n validateMinute() exception, minute: " << minute;
        throw range_error ("minutes must be between 0 and 1439");
    }
}

void LightScheduler::validateDay(int day)
{
    if (day < Monday || day > Everyday)
    {
        cout << "\n validateDay() exception, day: " << day;
        throw range_error ("day must be between Monday and Everyday");
    }
}

void LightScheduler::validateDayMinute(int day, int minute)
{
    validateDay(day);
    validateMinute(minute);
}

void LightScheduler::ScheduleTurnOn(int id, TimeStatus day, int minute)
{
    validateDayMinute(day, minute);

    scheduledLightEvents.push_back(ScheduledLightEvent(id, day, minute, LightStateOn));
}

void LightScheduler::ScheduleTurnOff(int id, TimeStatus day, int minute)
{
    validateDayMinute(day, minute);

    scheduledLightEvents.push_back(ScheduledLightEvent(id, day, minute, LightStateOff));
}

void LightScheduler::RemoveSchedule()
{

}

void LightScheduler::WakeUp()
{
    for(vector<ScheduledLightEvent>::iterator it = scheduledLightEvents.begin();
        it != scheduledLightEvents.end(); it++)
    {
        if((it->day == timeService.getTime().dayOfWeek ||
            it->day == Everyday) &&
           it->minuteOfDay == timeService.getTime().minuteOfDay)
        {
            switch (it->lightStatus)
            {
                case LightStateOn:
                    lightController.turnOn(it->id);
                    break;
	      
                case LightStateOff:
                    lightController.turnOff(it->id);
                    break;
	      
                default:
                    break;
            }
        }
    }
}

