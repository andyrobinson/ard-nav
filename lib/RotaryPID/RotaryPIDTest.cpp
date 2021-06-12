#include "RotaryPID.h"
#include "gtest/gtest.h"
#include "StubLogger.h"
//#include <iostream>

// based on an examination of logs 16192
// all values are per sample interval, in this case 0.2
#define MOMENTUM 0.85 // we don't retain angular momentum much, but this is 0.2s
#define RUDDER_AFFECT 0.3 // i.e each degree of rudder results in 1.5 degress of turn per second
#define MAX_ROTATION 15.0 // 75 degrees per second
#define CONVERGENCE_ITERATIONS 50 // 10 seconds
#define SAMPLE_INTERVAL_MS 200
#define HEADING_TOLERANCE 2

using namespace Utility;

namespace {

RotaryPID rotaryPID;
Switches switches_stub;
StubLogger stub_logger;

class RotaryPIDTest : public ::testing::Test {
 protected:
  RotaryPIDTest() {
  }

  void SetUp() override {

    rotaryPID = RotaryPID(45, &switches_stub, &stub_logger);
    switches_stub.set(0);
    switches_stub.set_percent(50.0);
  }

  void assert_convergence(uangle desired_heading, uangle current_heading, float momentum_factor) {
     int j = 0;
     angle output;
     float rotary_velocity = 0.0;
     int sample_factor = 1000/SAMPLE_INTERVAL_MS;
     EXPECT_LE(momentum_factor, 1.0); // momentum can only decrease on it's own
     // std::cout << "begin " << desired_heading << "\n";
     while (j < CONVERGENCE_ITERATIONS && abs1(udiff(current_heading,desired_heading)) > HEADING_TOLERANCE) {
       output = rotaryPID.calculate(desired_heading,current_heading,SAMPLE_INTERVAL_MS);
       rotary_velocity = (rotary_velocity * momentum_factor) - (((float) output) * RUDDER_AFFECT);
       rotary_velocity = min1(rotary_velocity, (float) MAX_ROTATION);
       current_heading = uadd(current_heading,(short) rotary_velocity);
       // std::cout << output << "," << current_heading << "\n";
       j++;
     }
     EXPECT_LT(j,CONVERGENCE_ITERATIONS);
     EXPECT_LE(abs1(udiff(current_heading,desired_heading)), HEADING_TOLERANCE);
  }
};

TEST_F(RotaryPIDTest, Should_converge_on_heading_at_all_compass_points_clockwise_turns) {
    for (int i = 0;i < 360; i+=30) {
        uangle current_heading = uadd(i,- 10);
        uangle desired_heading = uadd(i,10);
        assert_convergence(desired_heading,current_heading,0.0);
  }
}

TEST_F(RotaryPIDTest, Should_converge_on_heading_at_all_compass_points_anticlockwise_turns) {
    for (int i = 360;i > 0; i-=30) {
        uangle current_heading = uadd(i,10);
        uangle desired_heading = uadd(i,-10);
        assert_convergence(desired_heading,current_heading,0.0);
  }
}

TEST_F(RotaryPIDTest, Should_converge_on_heading_when_there_is_momentum) {
    for (int i = 0;i < 360; i+=30) {
        uangle current_heading = uadd(i,-10);
        uangle desired_heading = uadd(i,10);
        assert_convergence(desired_heading,current_heading, MOMENTUM);
  }
}

TEST_F(RotaryPIDTest, Should_converge_on_heading_for_large_turn_after_a_period_of_straight) {
    for (int i = 0;i < 10; i++) {
        assert_convergence(90,0, MOMENTUM);
    }
    assert_convergence(270,0, 0.8);
}

TEST_F(RotaryPIDTest, Should_not_have_output_bounce_when_changing_course) {
    for (int i = 0;i < 10; i++) {
        assert_convergence(190,0, MOMENTUM);
    }
    angle output = rotaryPID.calculate(170,190,200);
    EXPECT_LE(output, 20);
}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
