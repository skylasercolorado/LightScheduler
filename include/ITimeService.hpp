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
        typedef Subject<TimeServiceEvents> SubjectType;

        ITimeService()
        {
            std::cout << "\n ITimeService \n";
        }

        virtual LightSchedulerTime& GetTime() = 0;
        virtual void SetAlarmPeriod(uint periodInSeconds) = 0;
        virtual ObserverHandle<TimeServiceEvents>
                RegisterForTimeServiceEvent(TimeServiceEvents event,
                                            uint alarmPeriod,
                                            SubjectType::EventHandler notificationHandler) = 0;
        virtual bool UnregisterForTimeServiceEvent(ObserverHandle<TimeServiceEvents> handle) = 0;

        // Utility functions (Not part of the specified interface).
        static void ValidateMinute(int minute);
        static void ValidateDay(Day day);
        static void ValidateDayMinute(int day, int minute);
    };
}

#endif // CAMAX_ITIMESERVICE_HPP
