#include "LightScheduler.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace Camax;
using namespace std;

void LightScheduler::ScheduleTurnOn(int id, Day day, int minute)
{
    ITimeService::validateDayMinute(day, minute);

    scheduledLightEvents.push_back(ScheduledLightEvent(id, day, minute, LightStateOn));
}

void LightScheduler::ScheduleTurnOff(int id, Day day, int minute)
{
    ITimeService::validateDayMinute(day, minute);

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
        int today = timeService.getTime().dayOfWeek;
        int thisMinute = timeService.getTime().minuteOfDay;

        if((it->day == today ||
            it->day == Everyday ||
            (it->day == Weekend && (today == Saturday || today == Sunday )) ||
            (it->day == Weekday && (today >= Monday && today <= Friday ))
           ) && it->minuteOfDay == thisMinute)
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

void LightScheduler::NotificationHandler(ITimeService::TSEvents event)
{
    switch(event)
    {
        case ITimeService::TSEvents::AlarmActive:
            this->WakeUp();
            break;

        case ITimeService::TSEvents::Error:
        default:
            cout << "\nNotification Handler, Lightscheduler, error\n";
            break;
    }
}
