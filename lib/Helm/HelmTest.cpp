#include "Helm.h"
#include "Rudder.h"
#include "gtest/gtest.h"

namespace {

Rudder stub_rudder;
Compass stub_compass;
Helm helm(&stub_rudder, &stub_compass);

class HelmTest : public ::testing::Test {
 protected:
  HelmTest() {}
};

TEST_F(HelmTest, Stub_rudder_should_record_last_position) {
  stub_rudder.set_position(27);
  EXPECT_EQ(stub_rudder.get_position(), 27);
}

TEST_F(HelmTest, Should_steer_right_towards_the_requested_heading_using_half_the_difference) {
  stub_compass.set_bearing(0);
  stub_rudder.set_position(0);
  helm.steer(30);
  EXPECT_EQ(stub_rudder.get_position(), -15);
}

TEST_F(HelmTest, Should_steer_left_towards_the_requested_heading_using_half_the_difference) {
  stub_compass.set_bearing(190);
  stub_rudder.set_position(0);
  helm.steer(170);
  EXPECT_EQ(stub_rudder.get_position(), 10);
}

// TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement) {
//   stub_compass.set_bearing(0);
//   stub_rudder.set_position(0);
//   helm.steer(170);
//   EXPECT_EQ(stub_rudder.get_position(), -45);
// }


// steer repeatedly over the specified period

// correct the course if the current rudder is too ineffective

// (introduce the absolute wind direction)
// tack if the requested course is in the no-go zone
// choose the longer tack first

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
