#include "RotaryPID.h"
#include "gtest/gtest.h"
#include <iostream>

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

  void assert_convergence(uangle desired_heading, uangle current_heading) {
     int j = 0;
     angle output;
     uangle tolerance = 2;
     while (j < 30 && abs1(udiff(current_heading,desired_heading)) > tolerance) {
       output = rotaryPID.calculate(desired_heading,current_heading,200);
       current_heading = current_heading  - (short) ((float) output/5.0); // cod boat physics
       std::cout << output << "," << current_heading << "\n";
       j++;
     }
     EXPECT_LE(abs1(udiff(current_heading,desired_heading)), tolerance);
  }
};

TEST_F(RotaryPIDTest, Should_converge_on_heading_at_all_compass_points_clockwise_turns) {
    for (int i = 0;i < 360; i+=30) {
        uangle current_heading = i - 10;
        uangle desired_heading = i + 10;
        std::cout << "**Desired: " << desired_heading << "\n";
        assert_convergence(desired_heading,current_heading);
  }
}

TEST_F(RotaryPIDTest, DISABLED_Should_converge_on_heading_at_all_compass_points_with_momentum) {}
TEST_F(RotaryPIDTest, DISABLED_Should_never_exceed_limit) {}



}  //namespace



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
