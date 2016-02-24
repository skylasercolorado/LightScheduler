#include "LightScheduler.hpp"
#include <vector>
#include <iostream>

using namespace Camax;
using namespace std;

void LightScheduler::ScheduleTurnOn(int id, TimeStatus day, int minute)
{
  TimeService::validateDay(day);
  TimeService::validateMinute(minute);

  scheduledLightEvents.push_back(ScheduledLightEvent(id, day, minute, LightStateOn));
}

void LightScheduler::ScheduleTurnOff(int id, TimeStatus day, int minute)
{
  TimeService::validateDay(day);
  TimeService::validateMinute(minute);

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

