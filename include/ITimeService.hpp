#ifndef CAMAX_ITIMESERVICE_HPP
#define CAMAX_ITIMESERVICE_HPP

#include <iostream>
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
        Unknown = -1 // Marker of last element in enumeration
    };

    struct Time
    {
        int minuteOfDay;
        int dayOfWeek;
    };

    class ITimeService
    {
    public:
        virtual Time& getTime() = 0;
        virtual void setPeriodicAlarm() = 0;
        static void validateMinute(int minute);
        static void validateDay(Day day);
        static void validateDayMinute(int day, int minute);
    };
}

#endif // CAMAX_ITIMESERVICE_HPP
