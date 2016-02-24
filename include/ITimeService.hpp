#ifndef CAMAX_ITIMESERVICE_HPP
#define CAMAX_ITIMESERVICE_HPP

namespace Camax
{
  enum TimeStatus
    {
      Monday,
      Tuesday,
      Wednesday,
      Thursday,
      Friday,
      Saturday,
      Sunday,
      Everyday,
      TimeUnknown // Marker of last element in enumeration
    };

  struct Time
   {
     int minuteOfDay;
     int dayOfWeek;
   };

  class ITimeService
  {
    virtual Time& getTime() = 0;
    virtual void setPeriodicAlarm() = 0;
  };
}

#endif // CAMAX_ITIMESERVICE_HPP
