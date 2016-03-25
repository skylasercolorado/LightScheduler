#ifndef CAMAX_ITIMESERVICE_HPP
#define CAMAX_ITIMESERVICE_HPP

#include <iostream>
#include "Subject.hpp"
using namespace std;

namespace Camax
{
    enum Day : int
    {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday,
        Everyday,
        Weekday,
        Weekend,
        Unknown = -1 // Marker of last element in enumeration
    };

    struct LightSchedulerTime
    {
        int minuteOfDay;
        int dayOfWeek;
    };

    class ITimeService
    {
    public:
        enum class TimeServiceEvents { AlarmActive, Error};

        virtual LightSchedulerTime& GetTime() = 0;
        virtual void SetAlarmPeriod(uint periodInSeconds) = 0;
        static void ValidateMinute(int minute);
        static void ValidateDay(Day day);
        static void ValidateDayMinute(int day, int minute);
        void RegisterObserver(TimeServiceEvents event, std::function<void(TimeServiceEvents)> fn);
        void NotifyObservers(TimeServiceEvents event);

    private:
        Subject<TimeServiceEvents> events_;
    };
}

#endif // CAMAX_ITIMESERVICE_HPP
