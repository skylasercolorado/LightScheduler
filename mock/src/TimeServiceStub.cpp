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

void TimeServiceStub::NotifyObservers(TimeServiceEvents event)
{
    events_.Notify(event);
}

bool TimeServiceStub::FindObserver(ObserverHandle<TimeServiceEvents> handle)
{
    return events_.FindObserver(handle);
}

ObserverHandle<ITimeService::TimeServiceEvents>
TimeServiceStub::RegisterForTimeServiceEvent(TimeServiceEvents event,
                                             uint alarmPeriod,
                                             SubjectType::EventHandler notificationHandler)
{
    SetAlarmPeriod(alarmPeriod);
    return events_.RegisterObserver(event, notificationHandler);
}

bool TimeServiceStub::UnregisterForTimeServiceEvent(ObserverHandle<TimeServiceEvents> handle)
{
    if(events_.UnregisterObserver(handle))
    {
        SetAlarmPeriod(0);
        return true;
    }

    return false;
}