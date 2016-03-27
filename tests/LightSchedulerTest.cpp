#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "LightScheduler.hpp"
#include "LightControllerStub.hpp"
#include "TimeServiceStub.hpp"
#include <functional>
#include <system_error>

using namespace Camax;
using namespace std;
using namespace std::placeholders;

class LightSchedulerTest : public ::testing::Test
{
    public:

    const int AlarmPeriod = 60;


    LightSchedulerTest() : lightScheduler_(timeServiceStub_, lightControllerStub_)
    {
        lightScheduler_.RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents::AlarmActive, AlarmPeriod);
    }

    virtual void SetUp()
    {
        LightControllerStub::Reset();
        TimeServiceStub::Reset();
    }

    virtual void TearDown()
    {

    }

    void SetTimeTo(Day day, int minute);
    void CheckLightState(int id, LightStatus lightStatus);

    LightScheduler lightScheduler_;
    LightControllerStub lightControllerStub_;
    TimeServiceStub timeServiceStub_;
};

void LightSchedulerTest::SetTimeTo(Day day, int minute)
{
    timeServiceStub_.SetDay(day);
    timeServiceStub_.SetMinute(minute);
}

void LightSchedulerTest::CheckLightState(int id, LightStatus lightStatus)
{
    EXPECT_EQ(id, lightControllerStub_.GetLastId());
    EXPECT_EQ(lightStatus, lightControllerStub_.GetLastState());
}

// Testing the light controller stub after Reset
TEST_F(LightSchedulerTest, NoChangeToLightsDuringInitialization)
{
    EXPECT_EQ(LightIdUnknown, lightControllerStub_.GetLastId());
    EXPECT_EQ(LightStateUnknown, lightControllerStub_.GetLastState());
}

// Testing the time_ service stub after Reset
TEST_F(LightSchedulerTest, Create)
{
    EXPECT_EQ(Unknown, timeServiceStub_.GetTime().minuteOfDay);
    EXPECT_EQ(Unknown, timeServiceStub_.GetTime().dayOfWeek);
}

// Testing get/set of time_ service stub
TEST_F(LightSchedulerTest, Set)
{
    timeServiceStub_.SetMinute(42);
    timeServiceStub_.SetDay(Saturday);

    EXPECT_EQ(42, timeServiceStub_.GetTime().minuteOfDay);
    EXPECT_EQ(Saturday, timeServiceStub_.GetTime().dayOfWeek);
}

TEST_F(LightSchedulerTest, ScheduleOnEverydayNotTimeYet)
{
    lightScheduler_.ScheduleTurnOn(3, Everyday, 1200);
    SetTimeTo(Monday, 1199);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleOnEverydayItsTime)
{
    lightScheduler_.ScheduleTurnOn(3, Everyday, 1200);
    SetTimeTo(Monday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleOffEverydayItsTime)
{
    lightScheduler_.ScheduleTurnOff(3, Everyday, 1200);
    SetTimeTo(Monday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateOff);
}


TEST_F(LightSchedulerTest, NoScheduleNothingHappens)
{
    SetTimeTo(Monday, 100);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleTuesdayButItsMonday)
{
    lightScheduler_.ScheduleTurnOn(3, Tuesday, 1200);
    SetTimeTo(Monday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleTuesdayAndItsTuesday)
{
    lightScheduler_.ScheduleTurnOn(3, Tuesday, 1200);
    SetTimeTo(Tuesday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsFriday)
{
    lightScheduler_.ScheduleTurnOn(3, Weekend, 1200);
    SetTimeTo(Friday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsSaturday)
{
    lightScheduler_.ScheduleTurnOn(3, Weekend, 1200);
    SetTimeTo(Saturday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekEndItsMonday)
{
    lightScheduler_.ScheduleTurnOn(3, Weekend, 1200);
    SetTimeTo(Monday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, ScheduleWeekdayItsMonday)
{
    lightScheduler_.ScheduleTurnOn(3, Weekday, 1200);
    SetTimeTo(Monday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, ScheduleWeekdayItsSaturday)
{
    lightScheduler_.ScheduleTurnOn(3, Weekday, 1200);
    SetTimeTo(Saturday, 1200);
    lightScheduler_.WakeUp();

    CheckLightState(LightIdUnknown, LightStateUnknown);
}

TEST_F(LightSchedulerTest, CallbackThroughObserverPattern)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    // No observer is registered for the Error event, so the Subject will thrown an exception because of that.
    EXPECT_THROW(timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::Error), std::out_of_range);

    // Register for the 'error' event and test the correct exception is thrown
    lightScheduler_.RegisterForTimeServiceEvent(ITimeService::TimeServiceEvents::Error, AlarmPeriod);
    EXPECT_THROW(timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::Error), std::runtime_error);

    CheckLightState(3, LightStateOn);
    EXPECT_EQ(AlarmPeriod, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, SetAndGetAlaramPeriod)
{
    timeServiceStub_.SetAlarmPeriod(23);

    EXPECT_EQ(23, timeServiceStub_.GetAlarmPeriod());
}
