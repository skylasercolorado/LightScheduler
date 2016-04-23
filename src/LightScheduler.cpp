#include "LightScheduler.hpp"
#include "TimeServiceStub.hpp"
#include <system_error>
#include <algorithm>

using namespace Camax;
using namespace std;
using namespace std::placeholders;

void LightScheduler::ScheduleTurnOn(uint id, Day day, uint minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.push_back(ScheduledLightEvent(id, day, minute, LightStateOn));
}

void LightScheduler::ScheduleTurnOff(uint id, Day day, uint minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.push_back(ScheduledLightEvent(id, day, minute, LightStateOff));
}

void LightScheduler::RemoveSchedule(uint id, Day day, uint minute)
{
    ITimeService::ValidateDayMinute(day, minute);

    scheduledLightEvents_.erase(std::remove_if(scheduledLightEvents_.begin(), scheduledLightEvents_.end(),
                                               [&, this](ScheduledLightEvent x)
                                               {
                                                   return this->compare(ScheduledLightEvent(id, day, minute), x);
                                               }),
                                scheduledLightEvents_.end());
}

bool LightScheduler::compare(ScheduledLightEvent event, ScheduledLightEvent x)
{

    return event.id_ == x.id_ &&
           event.day_ == x.day_ &&
           event.minuteOfDay_ == x.minuteOfDay_;
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
    ILightController *lightController = nullptr;
//    std::map<uint, ILightController*>::iterator it = lightController_.find(event->id_);
    LightControllers::iterator it = lightController_.find(event->id_);
    if(it != lightController_.end())
        lightController = lightController_.find(event->id_)->second;
    else
        throw invalid_argument("Could not find lightController.");

    switch (event->lightStatus_)
    {
        case LightStateOn:
            lightController->TurnOn(event->id_);
            break;

        case LightStateOff:
            lightController->TurnOff(event->id_);
            break;

        default:
            break;
    }
}

bool LightScheduler::doesLightOperateNow(vector<Camax::ScheduledLightEvent>::iterator &event)
{
    Day today = timeService_.GetTime().dayOfWeek;
    uint thisMinute = timeService_.GetTime().minuteOfDay;

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

void LightScheduler::AddLightController(uint id, ILightController *lightController)
{
    if(lightController_.find(id) != lightController_.end())
        lightController_.at(id) = lightController;
    else
        lightController_.insert({id, lightController});
}
