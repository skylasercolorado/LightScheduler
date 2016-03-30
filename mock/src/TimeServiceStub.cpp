#include "LightScheduler.hpp"
#include "TimeServiceStub.hpp"
#include "TimeServiceStub.hpp"
#include <stdexcept>
#include <iostream>

using namespace Camax;
using namespace std;

LightSchedulerTime TimeServiceStub::time_ = {Unknown, Unknown};
int TimeServiceStub::periodInSeconds_ = 0;

void TimeServiceStub::Reset()
{
    time_.minuteOfDay = Unknown;
    time_.dayOfWeek = Unknown;
}

LightSchedulerTime & TimeServiceStub::GetTime()
{
    return time_;
}

void TimeServiceStub::SetAlarmPeriod(uint periodInSeconds)
{
    this->periodInSeconds_ = periodInSeconds;
}

void TimeServiceStub::SetMinute(int minute)
{
    ValidateMinute(minute);
    time_.minuteOfDay = minute;
}

void TimeServiceStub::SetDay(int day)
{
    ValidateDay((Monday));
    time_.dayOfWeek = day;
}

int TimeServiceStub::GetAlarmPeriod()
{
    return periodInSeconds_;
}

ObserverHandle<ITimeService::TimeServiceEvents> TimeServiceStub::RegisterObserver(ITimeService::TimeServiceEvents event,
                                                                    SubjectType::EventHandler fn)
{
    return events_.RegisterObserver(event, fn);
}

void TimeServiceStub::NotifyObservers(TimeServiceEvents event)
{
    TimeServiceStub::events_.Notify(event);
}

bool TimeServiceStub::UnregisterObserver(ObserverHandle<ITimeService::TimeServiceEvents> handle)
{
    return TimeServiceStub::events_.UnregisterObserver(handle);
}

bool TimeServiceStub::FindObserver(ObserverHandle<ITimeService::TimeServiceEvents> handle)
{
    return TimeServiceStub::events_.FindObserver(handle);
}

//TODO: Duplicate function. Keep the one above.
ObserverHandle<ITimeService::TimeServiceEvents>
TimeServiceStub::RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents event,
                                             uint alarmPeriod,
                                             SubjectType::EventHandler notificationHandler)
{
    SetAlarmPeriod(alarmPeriod);
//    return RegisterObserver(event, notificationHandler);
   return events_.RegisterObserver(event, notificationHandler);
}

//TODO: Duplicate function. Keep the one above.
bool TimeServiceStub::UnregisterForTimeServiceEvent(ObserverHandle<ITimeService::TimeServiceEvents> handle)
{
//    if(UnregisterObserver(handle))
    if(events_.UnregisterObserver(handle))
    {
        SetAlarmPeriod(0);
        return true;
    }

    return false;
}