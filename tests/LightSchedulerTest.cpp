#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "LightScheduler.hpp"
#include "LightControllerStub.hpp"
#include "TimeServiceStub.hpp"
#include <functional>

using namespace Camax;
using namespace std;
using namespace std::placeholders;

class LightSchedulerTest : public ::testing::Test
{
    public:

    const int AlarmPeriod = 60;


    LightSchedulerTest() : lightScheduler(timeServiceStub, lightControllerStub)
    {
        lightScheduler.RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents::AlarmActive);
    }

    virtual void SetUp()
    {
        LightControllerStub::reset();
        TimeServiceStub::reset();
    }

    virtual void TearDown()
    {

    }

    LightScheduler lightScheduler;
    LightControllerStub lightControllerStub;
    TimeServiceStub timeServiceStub;
    void setTimeTo(Day day, int minute);
    void checkLightState(int id, LightStatus lightStatus);
};

void LightSchedulerTest::setTimeTo(Day day, int minute)
{
    timeServiceStub.setDay(day);
    timeServiceStub.setMinute(minute);
}

void LightSchedulerTest::checkLightState(int id, LightStatus lightStatus)
{
    EXPECT_EQ(id, lightControllerStub.getLastId());
    EXPECT_EQ(lightStatus, lightControllerStub.getLastState());
}

// Testing the light controller stub after reset
TEST_F(LightSchedulerTest, NoChangeToLightsDuringInitialization)
{
    EXPECT_EQ(LightIdUnknown, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateUnknown, lightControllerStub.getLastState());
}

// Testing the time service stub after reset
TEST_F(LightSchedulerTest, Create)
{
    EXPECT_EQ(Unknown, timeServiceStub.getTime().minuteOfDay);
    EXPECT_EQ(Unknown, timeServiceStub.getTime().dayOfWeek);
}

// Testing get/set of time service stub
TEST_F(LightSchedulerTest, Set)
{
    timeServiceStub.setMinute(42);
    timeServiceStub.setDay(Saturday);

    EXPECT_EQ(42, timeServiceStub.getTime().minuteOfDay);
    EXPECT_EQ(Saturday, timeServiceStub.getTime().dayOfWeek);
}

TEST_F(LightSchedulerTest, ScheduleOnEverydayNotTimeYet)
{
    lightScheduler.ScheduleTurnOn(3, Everyday, 1200);
    setTimeTo(Monday, 1199);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleOnEverydayItsTime)
{
    lightScheduler.ScheduleTurnOn(3, Everyday, 1200);
    setTimeTo(Monday, 1200);
    lightScheduler.WakeUp();

    checkLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleOffEverydayItsTime)
{
    lightScheduler.ScheduleTurnOff(3, Everyday, 1200);
    setTimeTo(Monday, 1200);
    lightScheduler.WakeUp();

    checkLightState(3, LightStateOff);
}


TEST_F(LightSchedulerTest, NoScheduleNothingHappens)
{
    setTimeTo(Monday, 100);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleTuesdayButItsMonday)
{
    lightScheduler.ScheduleTurnOn(3, Tuesday, 1200);
    setTimeTo(Monday, 1200);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleTuesdayAndItsTuesday)
{
    lightScheduler.ScheduleTurnOn(3, Tuesday, 1200);
    setTimeTo(Tuesday, 1200);
    lightScheduler.WakeUp();

    checkLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsFriday)
{
    lightScheduler.ScheduleTurnOn(3, Weekend, 1200);
    setTimeTo(Friday, 1200);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsSaturday)
{
    lightScheduler.ScheduleTurnOn(3, Weekend, 1200);
    setTimeTo(Saturday, 1200);
    lightScheduler.WakeUp();

    checkLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsMonday)
{
    lightScheduler.ScheduleTurnOn(3, Weekend, 1200);
    setTimeTo(Monday, 1200);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleWeekdayItsMonday)
{
    lightScheduler.ScheduleTurnOn(3, Weekday, 1200);
    setTimeTo(Monday, 1200);
    lightScheduler.WakeUp();

    checkLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekdayItsSaturday)
{
    lightScheduler.ScheduleTurnOn(3, Weekday, 1200);
    setTimeTo(Saturday, 1200);
    lightScheduler.WakeUp();

    checkLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, CallbackThroughObserverPattern)
{
    lightScheduler.ScheduleTurnOn(3, Monday, 1200);
    setTimeTo(Monday, 1200);

    timeServiceStub.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    // No observer is registered for the Error event, so the Subject will thrown an exception because of that.
    EXPECT_ANY_THROW(timeServiceStub.NotifyObservers(ITimeService::TimeServiceEvents::Error));

    checkLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, SetAndGetAlaramPeriod)
{
    timeServiceStub.setPeriodicAlarm(23);

    EXPECT_EQ(23, timeServiceStub.getAlarmPeriod());
}
