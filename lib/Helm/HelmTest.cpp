#include "Helm.h"
#include "Rudder.h"
#include "gtest/gtest.h"

namespace {

Rudder stub_rudder;
Compass stub_compass;
Timer stub_timer;
WindSensor stub_windsensor;
Sail stub_sail;
Helm helm;
Logger logger;
angle wind_sample[] = {180};

class HelmTest : public ::testing::Test {
 protected:
  HelmTest() {
    stub_rudder.reset();
    stub_sail.reset();
    stub_windsensor.set_relative(wind_sample, 1);
    helm = Helm(&stub_rudder, &stub_compass, &stub_timer, &stub_windsensor, &stub_sail, &logger);
  }

  void SetUp() override {
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

TEST_F(HelmTest, stub_windsensor_should_return_readings_sticking_on_last) {
  angle wind_values[] = {4,5};
  stub_windsensor.set_relative(wind_values,2);
  EXPECT_EQ(stub_windsensor.relative(),4);
  EXPECT_EQ(stub_windsensor.relative(),5);
  EXPECT_EQ(stub_windsensor.relative(),5);
}

TEST_F(HelmTest, Should_steer_right_towards_the_requested_heading_using_half_the_difference) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(30, 1, 1);
  EXPECT_EQ(rudder_position(), -15);
}

TEST_F(HelmTest, Should_steer_left_towards_the_requested_heading_using_half_the_difference) {
  uangle bearing = 190;
  stub_compass.set_bearings(&bearing,2);
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
  uangle bearings[] = {180, 180};
  stub_compass.set_bearings(bearings,1);
  helm.steer(10, 2, 1);
  EXPECT_EQ(stub_rudder.get_positions()[1], 45);
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
  EXPECT_EQ(positions[1],-20 - NOT_TURNING_NUDGE_DEGREES);
  EXPECT_EQ(positions[2],-20 - 2*NOT_TURNING_NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_steer_further_only_when_not_turning) {
  uangle bearings[] = {20,10,10};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(350, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],15);
  EXPECT_EQ(positions[1],10);
  EXPECT_EQ(positions[2],10 + NOT_TURNING_NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_consider_very_small_changes_as_not_turning) {
  uangle bearings[] = {10,8,6};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(350, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],10);
  EXPECT_EQ(positions[1],10 + NOT_TURNING_NUDGE_DEGREES);
  EXPECT_EQ(positions[2],10 + NOT_TURNING_NUDGE_DEGREES*2);
}

TEST_F(HelmTest, Should_maintain_rudder_position_if_on_course) {
  uangle bearings[] = {210,200,200};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(200, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],5);
  EXPECT_EQ(positions[1],0);
  EXPECT_EQ(positions[2],0);
}

TEST_F(HelmTest, Should_maintain_non_zero_rudder_position_if_approx_on_course) {
  uangle bearings[] = {130,152,149};
  stub_compass.set_bearings(bearings, 3);

  helm.steer(150, 3, 1);

  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],-10);
  EXPECT_EQ(positions[1],1);
  EXPECT_EQ(positions[2],1);
}

TEST_F(HelmTest, Should_set_the_sail_every_time_we_steer) {
  uangle bearings[] = {210,190};
  angle wind[] = {70, 90};
  stub_compass.set_bearings(bearings, 2);
  stub_windsensor.set_relative(wind, 2);

  helm.steer(200, 3, 1);

  angle *positions = stub_rudder.get_positions();
  angle *sail_calls = stub_sail.get_calls();

  EXPECT_EQ(positions[0],5);
  EXPECT_EQ(positions[1],-5);
  EXPECT_EQ(sail_calls[0],wind[0]);
  EXPECT_EQ(sail_calls[1],wind[1]);

}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
