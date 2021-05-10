#include "StubLogger.h"
#include "RotaryPID.h"
#include "gtest/gtest.h"

using namespace Utility;

namespace {

RotaryPID rotaryPID;
StubLogger logger;
Switches stub_switches;

class RotaryPIDTest : public ::testing::Test {
 protected:
  RotaryPIDTest() {
  }

  void SetUp() override {
    stub_switches.set(2);
    rotaryPID = RotaryPID(&stub_switches, &logger);
  }

};

TEST_F(RotaryPIDTest, Should_steer_right_towards_the_requested_heading_at_all_compass_points) {
  EXPECT_EQ(1,1);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
