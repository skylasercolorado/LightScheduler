#ifndef CAMAX_ITIMESERVICE_HPP
#define CAMAX_ITIMESERVICE_HPP

#include <iostream>
#include "Subject.hpp" //TODO: Remove
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

        // Utility functions (Not part of the specified interface).
        static void ValidateMinute(int minute);
        static void ValidateDay(Day day);
        static void ValidateDayMinute(int day, int minute);

        // TODO: These Observer methods should be moved to the mock.
        ObserverHandle<TimeServiceEvents> RegisterObserver(TimeServiceEvents event,
                                                           Subject<ITimeService::TimeServiceEvents>::EventHandler fn);
        bool FindObserver(ObserverHandle<ITimeService::TimeServiceEvents> handle);
        void NotifyObservers(TimeServiceEvents event);
        bool UnregisterObserver(ObserverHandle<TimeServiceEvents> handle);

    private:
        // TODO: This subject object should be moved to the mock.
        Subject<TimeServiceEvents> events_;
    };
}

#endif // CAMAX_ITIMESERVICE_HPP
