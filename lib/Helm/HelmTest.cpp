#include "Helm.h"
#include "Rudder.h"
#include "gtest/gtest.h"

namespace {

Rudder stub_rudder;
Compass stub_compass;
Timer stub_timer;
Helm helm(&stub_rudder, &stub_compass, &stub_timer);

class HelmTest : public ::testing::Test {
 protected:
  HelmTest() {}

  void SetUp() override {
    stub_rudder.reset();
  }

  angle rudder_position() {
    return *stub_rudder.get_positions();
  }
};

TEST_F(HelmTest, Stub_rudder_should_record_last_position) {
  stub_rudder.set_position(27);
  EXPECT_EQ(rudder_position(), 27);
}

TEST_F(HelmTest, Stub_compass_should_return_bearing_set) {
  uangle bearing = 99;
  stub_compass.set_bearings(&bearing,1);
  EXPECT_EQ(stub_compass.bearing(), 99);
}

TEST_F(HelmTest, Should_steer_right_towards_the_requested_heading_using_half_the_difference) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(30, 1, 1);
  EXPECT_EQ(rudder_position(), -15);
}

TEST_F(HelmTest, Should_steer_left_towards_the_requested_heading_using_half_the_difference) {
  uangle bearing = 190;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(170, 1, 1);
  EXPECT_EQ(rudder_position(), 10);
}

TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_left) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(170, 1, 1);
  EXPECT_EQ(rudder_position(), -45);
}

TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_right) {
  uangle bearing = 180;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(10, 1, 1);
  EXPECT_EQ(rudder_position(), 45);
}

TEST_F(HelmTest, Should_steer_repeatedly_given_overall_time_and_wait_time) {
  uangle bearings[] = {0, 15, 30};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(30, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],-15);
  EXPECT_EQ(positions[1],-7);
  EXPECT_EQ(positions[2],0);
}

TEST_F(HelmTest, Should_steer_further_if_rudder_is_ineffective) {
  uangle bearings[] = {190,190,190};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(230, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],-20);
  EXPECT_EQ(positions[1],-40);
  EXPECT_EQ(positions[2],-45);
}

TEST_F(HelmTest, Should_steer_further_only_when_not_turning) {
  uangle bearings[] = {10,8,8};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(350, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],10);
  EXPECT_EQ(positions[1],9);
  EXPECT_EQ(positions[2],18);
}

// (introduce the absolute wind direction)
// tack if the requested course is in the no-go zone
// choose the longer tack first

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
