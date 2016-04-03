#include "LightScheduler.hpp"
#include "TimeServiceStub.hpp"
#include <system_error>
#include <algorithm>

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

void LightScheduler::RemoveSchedule(int id, Day day, int minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.erase(std::remove_if(scheduledLightEvents_.begin(), scheduledLightEvents_.end(),
                                               [&, this](ScheduledLightEvent x)
                                               {
                                                   return this->compare(ScheduledLightEvent(id, day, minute,LightStateOn), x);
                                               }),
                                scheduledLightEvents_.end());
}

bool LightScheduler::compare(ScheduledLightEvent event, ScheduledLightEvent x)
{

    if(x.id_ == event.id_ &&
       x.day_ == event.day_ &&
       x.minuteOfDay_ == event.minuteOfDay_)
        return true;
    else
        return false;
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
           ) && event->minuteOfDay_ == thisMinute;
}
void LightScheduler::destroy()
{
    if(!alreadyDestroyed_)
    {
        if(timeService_.UnregisterForTimeServiceEvent(observerHandle_))
            alreadyDestroyed_ = true;
    }
}

ObserverHandle<ITimeService::TimeServiceEvents> LightScheduler::getObserverHandle()
{
    return observerHandle_;
}
