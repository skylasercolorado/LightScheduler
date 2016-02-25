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
};

// Testing the light controller stub after reset
TEST_F(LightSchedulerTest, NoChangeToLightsDuringInitialization)
{
    EXPECT_EQ(LightIdUnknown, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateUnknown, lightControllerStub.getLastState());
}

// Testing the time service stub after reset
TEST_F(LightSchedulerTest, Create)
{
    EXPECT_EQ(TimeUnknown, timeServiceStub.getTime().minuteOfDay);
    EXPECT_EQ(TimeUnknown, timeServiceStub.getTime().dayOfWeek);
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
    timeServiceStub.setDay(Monday);
    timeServiceStub.setMinute(1199);
    lightScheduler.WakeUp();

    EXPECT_EQ(LightIdUnknown, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateUnknown, lightControllerStub.getLastState());
}

TEST_F(LightSchedulerTest, ScheduleOnEverydayItsTime)
{
    lightScheduler.ScheduleTurnOn(3, Everyday, 1200);
    timeServiceStub.setDay(Monday);
    timeServiceStub.setMinute(1200);
    lightScheduler.WakeUp();

    EXPECT_EQ(3, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateOn, lightControllerStub.getLastState());
}

TEST_F(LightSchedulerTest, ScheduleOffEverydayItsTime)
{
    lightScheduler.ScheduleTurnOff(3, Everyday, 1200);
    timeServiceStub.setDay(Monday);
    timeServiceStub.setMinute(1200);
    lightScheduler.WakeUp();

    EXPECT_EQ(3, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateOff, lightControllerStub.getLastState());
}


TEST_F(LightSchedulerTest, NoScheduleNothingHappens)
{
    timeServiceStub.setDay(Monday);
    timeServiceStub.setMinute(100);
    lightScheduler.WakeUp();

    EXPECT_EQ(LightIdUnknown, lightControllerStub.getLastId());
    EXPECT_EQ(LightStateUnknown, lightControllerStub.getLastState());
}


