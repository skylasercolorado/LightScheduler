#ifndef CAMAX_TIMESERVICE_HPP
#define CAMAX_TIMESERVICE_HPP

#include "ITimeService.hpp"

namespace Camax
{
  class TimeService : public ITimeService
  {
    public:
      Time& getTime();
      void setPeriodicAlarm();
      // The actual mock starts from hereon forward
      static void setMinute(int minute);
      static void setDay(int day);
      static void validateMinute(int minute);
      static void validateDay(int day);
      static void reset();

    private:
      static Time time;
  };
}

#endif // CAMAX_TIMESERVICE_HPP
