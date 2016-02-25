#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "LightScheduler.hpp"
#include "LightControllerStub.hpp"
#include "TimeServiceStub.hpp"

using namespace Camax;
using namespace std;

class LightSchedulerTest : public ::testing::Test
{
    public:
    LightSchedulerTest() : lightScheduler(timeServiceStub, lightControllerStub)
    {

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


