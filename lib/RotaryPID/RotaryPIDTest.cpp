#include "RotaryPID.h"
#include "gtest/gtest.h"

using namespace Utility;

namespace {

RotaryPID rotaryPID;
Switches stub_switches;

class RotaryPIDTest : public ::testing::Test {
 protected:
  RotaryPIDTest() {
  }

  void SetUp() override {
    stub_switches.set(2);
    rotaryPID = RotaryPID(45, &stub_switches);
  }

  void assert_convergence(uangle desired_heading, uangle current_heading, float momentum_factor) {
     int j = 0;
     angle output;
     float rotary_velocity = 0.0;
     uangle tolerance = 2;
     long sample_time_ms = 200;
     float max_rotation = 20.0; // rotation will not increase beyond a limit
     EXPECT_LE(momentum_factor, 1.0); // momentum can only decrease on it's own
     while (j < 30 && abs1(udiff(current_heading,desired_heading)) > tolerance) {
       output = rotaryPID.calculate(desired_heading,current_heading,sample_time_ms);
       rotary_velocity = (rotary_velocity * momentum_factor) - (((float) output)/5.0);
       rotary_velocity = min1(rotary_velocity, max_rotation);
       current_heading = uadd(current_heading,(short) rotary_velocity);
       j++;
     }
     EXPECT_LE(abs1(udiff(current_heading,desired_heading)), tolerance);
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
        assert_convergence(desired_heading,current_heading, 0.8);
  }
}
}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
