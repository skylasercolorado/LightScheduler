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

    const uint AlarmPeriod = 60;

    //TODO: These are runtime stubs. Change one of them to be a linktime stub.
//    LightSchedulerTest() : lightScheduler_(timeServiceStub_, lightControllerStub_, AlarmPeriod)
    LightSchedulerTest() : lightScheduler_(timeServiceStub_, AlarmPeriod)
    {
        observerHandle_ = lightScheduler_.getObserverHandle();
    }

    virtual void SetUp()
    {
        LightControllerStub::Reset();
        TimeServiceStub::Reset();
    }

    virtual void TearDown()
    {

    }

    void SetTimeTo(Day day, uint minute);
    void CheckLightState(uint id, LightStatus lightStatus);

    LightControllerStub lightControllerStub_;
    TimeServiceStub timeServiceStub_;
    ObserverHandle<ITimeService::TimeServiceEvents> observerHandle_;
    LightScheduler lightScheduler_;
};

void LightSchedulerTest::SetTimeTo(Day day, uint minute)
{
    timeServiceStub_.SetDay(day);
    timeServiceStub_.SetMinute(minute);
}

void LightSchedulerTest::CheckLightState(uint id, LightStatus lightStatus)
{
    EXPECT_EQ(lightStatus, lightControllerStub_.GetLightState(id));
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
    EXPECT_EQ(0, timeServiceStub_.GetTime().minuteOfDay);
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
    lightScheduler_.ScheduleTurnOn(4, Everyday, 1199);
    SetTimeTo(Monday, 1199);
    lightScheduler_.WakeUp();

    CheckLightState(3, LightStateUnknown);
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

    CheckLightState(3, LightStateUnknown);
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

    CheckLightState(3, LightStateUnknown);
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

    CheckLightState(3, LightStateUnknown);
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

    CheckLightState(3, LightStateUnknown);
}

TEST_F(LightSchedulerTest, CallbackThroughObserverPattern)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateOn);
    EXPECT_EQ(AlarmPeriod, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, NotRegisteredForTimeServiceEvent)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    // No observer is registered for the Error event, so the Subject will thrown an exception because of that.
    EXPECT_THROW(timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::Error), std::out_of_range);

    CheckLightState(3, LightStateOn);
    EXPECT_EQ(AlarmPeriod, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, UnregisterForAlarmActive)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    timeServiceStub_.UnregisterForTimeServiceEvent(observerHandle_);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(LightIdUnknown, LightStateUnknown);
    EXPECT_EQ(0, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, FindObserverTrue)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    EXPECT_TRUE(timeServiceStub_.FindObserver(observerHandle_));
    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateOn);
    EXPECT_EQ(AlarmPeriod, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, UnregisterForAlarmActiveAndCheck)
{
    lightScheduler_.ScheduleTurnOn(3, Monday, 1200);
    SetTimeTo(Monday, 1200);

    timeServiceStub_.UnregisterForTimeServiceEvent(observerHandle_);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);
    EXPECT_FALSE(timeServiceStub_.FindObserver(observerHandle_));

    CheckLightState(LightIdUnknown, LightStateUnknown);
    EXPECT_EQ(0, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, SetAndGetAlaramPeriod)
{
    timeServiceStub_.SetAlarmPeriod(23);

    EXPECT_EQ(23, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, CreateStartsOneMinuteAlarm)
{
    EXPECT_TRUE(timeServiceStub_.FindObserver(observerHandle_));
    EXPECT_EQ(AlarmPeriod, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, DestroyCancelsOneMinuteAlarm)
{
    lightScheduler_.destroy();
    EXPECT_FALSE(timeServiceStub_.FindObserver(observerHandle_));
    EXPECT_EQ(0, timeServiceStub_.GetAlarmPeriod());
}

TEST_F(LightSchedulerTest, ScheduleTwoEventsAtTheSameTime)
{
    lightScheduler_.ScheduleTurnOn(3, Sunday, 1200);
    lightScheduler_.ScheduleTurnOn(12, Sunday, 1200);

    SetTimeTo(Sunday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateOn);
    CheckLightState(12, LightStateOn);
}

//TEST_F(LightSchedulerTest, DestructorWithArtificialBlock)
//{
//    ObserverHandle<ITimeService::TimeServiceEvents> testHandle;
//
//    {
//        LightScheduler testInstance(timeServiceStub_, lightControllerStub_, AlarmPeriod);
//        testHandle = testInstance.getObserverHandle();
//
//        EXPECT_TRUE(timeServiceStub_.FindObserver(testHandle));
//    }
//
//    EXPECT_FALSE(timeServiceStub_.FindObserver(testHandle));
//}

//TEST_F(LightSchedulerTest, DestructorWithHelperFunction)
//{
//    ObserverHandle<ITimeService::TimeServiceEvents> testHandle;
//
//    LightScheduler testInstance(timeServiceStub_, lightControllerStub_, AlarmPeriod);
//    testHandle = testInstance.getObserverHandle();
//
//    EXPECT_TRUE(timeServiceStub_.FindObserver(testHandle));
//
//    testInstance.destroy();
//
//    EXPECT_FALSE(timeServiceStub_.FindObserver(testHandle));
//}

TEST_F(LightSchedulerTest, RemoveSchedule)
{
    lightScheduler_.ScheduleTurnOn(3, Sunday, 1200);

    lightScheduler_.RemoveSchedule(3, Sunday, 1200);

    SetTimeTo(Sunday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateUnknown);
}

TEST_F(LightSchedulerTest, RemoveMultipleScheduledEvent)
{
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);
    lightScheduler_.ScheduleTurnOn(7, Monday, 600);
    // Through gcc options, won't let app programmer enter negative values. These are all uints. gcc will halt.
    // lightScheduler_.ScheduleTurnOn(-17, Monday, 600);

    lightScheduler_.RemoveSchedule(6, Monday, 600);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(6, LightStateUnknown);
    CheckLightState(7, LightStateOn);
}

TEST_F(LightSchedulerTest, RemoveNonScheduledEvent)
{
    lightScheduler_.RemoveSchedule(6, Monday, 600);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(6, LightStateUnknown);
}

TEST_F(LightSchedulerTest, MultipleScheduledEventsForSameLight)
{
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);
    lightScheduler_.ScheduleTurnOff(6, Monday, 700);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);
    CheckLightState(6, LightStateOn);

    SetTimeTo(Monday, 700);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(6, LightStateOff);
}

TEST_F(LightSchedulerTest, MultipleScheduledEventsAtSameTime)
{
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);
    lightScheduler_.ScheduleTurnOn(7, Monday, 600);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(6, LightStateOn);
    CheckLightState(7, LightStateOn);
}

TEST_F(LightSchedulerTest, MultipleScheduledEventsAtSameTimeSameLight)
{
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(6, LightStateOn);
    CheckLightState(6, LightStateOn);
}

TEST_F(LightSchedulerTest, MultipleScheduledEventsAtSameTimeSameLightOnAndOff)
{
    lightScheduler_.ScheduleTurnOn(6, Monday, 600);
    lightScheduler_.ScheduleTurnOff(6, Monday, 600);

    SetTimeTo(Monday, 600);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    // Last schedulent event should prevail.
    CheckLightState(6, LightStateOff);
}

TEST_F(LightSchedulerTest, DayRightTimeRightRightLightTurnedOff)
{
    lightScheduler_.ScheduleTurnOff(3, Wednesday, 1200);
    SetTimeTo(Wednesday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateOff);
}

TEST_F(LightSchedulerTest, DayRightTimeRightRightLightTurnedOn)
{
    lightScheduler_.ScheduleTurnOn(3, Wednesday, 1200);
    SetTimeTo(Wednesday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateOn);
}

TEST_F(LightSchedulerTest, DayWrongTimeRightNoLightsChange)
{
    lightScheduler_.ScheduleTurnOn(3, Wednesday, 1200);
    SetTimeTo(Thursday, 1200);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateUnknown);
}

TEST_F(LightSchedulerTest, DayRightTimeWrongNoLightsChange)
{
    lightScheduler_.ScheduleTurnOn(3, Wednesday, 1200);
    SetTimeTo(Wednesday, 23);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateUnknown);
}

TEST_F(LightSchedulerTest, DayWrongTimeWrongNoLightsChange)
{
    lightScheduler_.ScheduleTurnOn(3, Wednesday, 1200);
    SetTimeTo(Thursday, 23);

    timeServiceStub_.NotifyObservers(ITimeService::TimeServiceEvents::AlarmActive);

    CheckLightState(3, LightStateUnknown);
}
