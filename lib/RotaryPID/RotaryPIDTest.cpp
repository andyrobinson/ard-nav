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

};

TEST_F(RotaryPIDTest, Should_compute_negative_output_towards_clockwise_headings_at_all_compass_points) {
  angle output;
  output = rotaryPID.calculate(5,10,500); // discard first one
  for (int i = 0;i < 405; i+=45) {
      output = rotaryPID.calculate(uadd(i,10),uadd(i,-10),500);
      std::cout << uadd(i,10);
      std::cout << ",";
      std::cout << uadd(i,-10);
      std::cout << ",";
      std::cout << output;
      std::cout << "\n";
      EXPECT_LT(output,0);
  }
}

TEST_F(RotaryPIDTest, Should_compute_positive_output_towards_anticlockwise_heading_at_all_compass_points) {
  angle output;
  output = rotaryPID.calculate(20,10,500); // discard first one
  for (int i = 0;i < 405; i+=45) {
      output = rotaryPID.calculate(uadd(i,-10),uadd(i,10),500);
      std::cout << uadd(i,-10);
      std::cout << ",";
      std::cout << uadd(i,10);
      std::cout << ",";
      std::cout << output;
      std::cout << "\n";
      EXPECT_GT(output,0);
  }
}
TEST_F(RotaryPIDTest, DISABLED_Should_converge_on_heading_at_all_compass_points) {}
TEST_F(RotaryPIDTest, DISABLED_Should_converge_on_heading_at_all_compass_points_with_momentum) {}
TEST_F(RotaryPIDTest, DISABLED_Should_never_exceed_limit) {}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
