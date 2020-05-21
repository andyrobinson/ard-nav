#include "StubLogger.h"
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
StubLogger logger;
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

TEST_F(HelmTest, Should_steer_right_towards_the_requested_heading_using_nudge_value) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(30, 1, 1);
  EXPECT_EQ(rudder_position(), -NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_steer_left_towards_the_requested_heading_using_the_nudge_value) {
  uangle bearings[] = {190, 190, 190};
  stub_compass.set_bearings(bearings, 3);
  helm.steer(170, 3000, 1000);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[2], NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_left) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(170, 20, 1);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],- NUDGE_DEGREES);
  EXPECT_EQ(positions[1],- 2 * NUDGE_DEGREES);
  EXPECT_EQ(positions[15],-45);
  EXPECT_EQ(positions[16],-45);
}

 TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_right) {
  uangle bearing = 180;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(10, 20000, 1000);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[1], 0);
  EXPECT_EQ(positions[2], NUDGE_DEGREES);
  EXPECT_EQ(positions[15], 45);
  EXPECT_EQ(positions[16], 45);
}

 TEST_F(HelmTest, Should_steer_repeatedly_given_overall_time_and_wait_time) {
   uangle bearings[] = {0, 10, 20};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(30, 3000, 1000); // bigger values required for slower rate of turn

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-2*NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-2*NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_steer_further_if_rudder_is_ineffective) {
   uangle bearings[] = {190,190,190};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(230, 4000, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-2 * NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-3 * NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_steer_further_only_when_not_turning) {
   uangle bearings[] = {20,10,10};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(350, 1500, 500);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],NUDGE_DEGREES);
   EXPECT_EQ(positions[1],NUDGE_DEGREES);
   EXPECT_EQ(positions[2],2 * NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_consider_very_small_changes_as_not_turning) {
   uangle bearings[] = {10,8,6};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(350, 3000, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],NUDGE_DEGREES);
   EXPECT_EQ(positions[1],2*NUDGE_DEGREES);
   EXPECT_EQ(positions[2],3*NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_ignore_single_compass_point_changes) {
   uangle bearings[] = {200,201,200};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(200, 300, 100);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],0);
   EXPECT_EQ(positions[1],0);
   EXPECT_EQ(positions[2],0);
 }

 TEST_F(HelmTest, Should_maintain_rudder_position_if_on_course) {
   uangle bearings[] = {210,200,200};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(200, 3000, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_maintain_non_zero_rudder_position_if_approx_on_course) {
   uangle bearings[] = {100,130,152,149};
   stub_compass.set_bearings(bearings, 4);

   helm.steer(150, 4000, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],4);
   EXPECT_EQ(positions[1],8);
   EXPECT_EQ(positions[2],12);
   EXPECT_EQ(positions[3],12);
 }

TEST_F(HelmTest, Rudder_deflection_should_be_progressive_through_turn) {
   uangle bearings[] = {200, 200, 210, 220, 240, 260, 280, 290, 300, 305};
   stub_compass.set_bearings(bearings, 10);

   helm.steer(300, 5500, 500);
   angle *positions = stub_rudder.get_positions();

   EXPECT_EQ(positions[1],-8);
   EXPECT_EQ(positions[2],-12);
   EXPECT_EQ(positions[3],-16);
   EXPECT_EQ(positions[4],-20);
   EXPECT_EQ(positions[5],-20);
   EXPECT_EQ(positions[6],-16);
   EXPECT_EQ(positions[7],-12);
   EXPECT_EQ(positions[8],-12);
   EXPECT_EQ(positions[9],-8);
   EXPECT_EQ(positions[10],-4);
}

TEST_F(HelmTest, Should_set_the_sail_every_time_we_steer) {
  uangle bearings[] = {210,190};
  angle wind[] = {70, 90};
  stub_compass.set_bearings(bearings, 2);
  stub_windsensor.set_relative(wind, 2);

  helm.steer(200, 3, 1);

  angle *positions = stub_rudder.get_positions();
  angle *sail_calls = stub_sail.get_calls();

  EXPECT_EQ(positions[0],- NUDGE_DEGREES);
  EXPECT_EQ(positions[1],- NUDGE_DEGREES * 2);
  EXPECT_EQ(sail_calls[0],wind[0]);
  EXPECT_EQ(sail_calls[1],wind[1]);
}
}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
