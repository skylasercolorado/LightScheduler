#include "LightScheduler.hpp"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace Camax;
using namespace std;
using namespace std::placeholders;

void LightScheduler::ScheduleTurnOn(int id, Day day, int minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.push_back(ScheduledLightEvent(id, day, minute, LightStateOn));
}

void LightScheduler::ScheduleTurnOff(int id, Day day, int minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.push_back(ScheduledLightEvent(id, day, minute, LightStateOff));
}

void LightScheduler::RemoveSchedule()
{

}

void LightScheduler::WakeUp()
{

    for(vector<ScheduledLightEvent>::iterator it = scheduledLightEvents_.begin();
        it != scheduledLightEvents_.end(); it++)
    {
        if(doesLightOperateNow(it))
            operateLight(it);
    }
}

void LightScheduler::operateLight(vector<Camax::ScheduledLightEvent>::iterator &event)
{
    switch (event->lightStatus_)
    {
        case LightStateOn:
            lightController_.TurnOn(event->id_);
            break;

        case LightStateOff:
            lightController_.TurnOff(event->id_);
            break;

        default:
            break;
    }
}

bool LightScheduler::doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator &event)
{
    int today = timeService_.GetTime().dayOfWeek;
    int thisMinute = timeService_.GetTime().minuteOfDay;

    return (event->day_ == today ||
            event->day_ == Everyday ||
            (event->day_ == Weekend && (today == Saturday || today == Sunday )) ||
            (event->day_ == Weekday && (today >= Monday && today <= Friday ))
           ) && event->minuteOfDay_ == thisMinute ? true : false;
}

void LightScheduler::NotificationHandler(ITimeService::TimeServiceEvents event)
{
    switch(event)
    {
        case ITimeService::TimeServiceEvents::AlarmActive:
            this->WakeUp();
            break;

        case ITimeService::TimeServiceEvents::Error:
            break;
    }
}

void LightScheduler::RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents event)
{
    timeService_.RegisterObserver(event, std::bind(&LightScheduler::NotificationHandler, this, _1));
}
