#include "StubLogger.h"
#include "Helm.h"
#include "Rudder.h"
#include "gtest/gtest.h"
#include "Utility.h"
#include "Switches.h"

using namespace Utility;

namespace {

Rudder stub_rudder;
Compass stub_compass;
Timer stub_timer;
WindSensor stub_windsensor;
Sail stub_sail;
Helm helm;
StubLogger logger;
Switches stub_switches;
angle wind_sample[] = {180};

class HelmTest : public ::testing::Test {
 protected:
  HelmTest() {
    stub_rudder.reset();
    stub_sail.reset();
    stub_windsensor.set_relative(wind_sample, 1);
    stub_switches.set(0);
    helm = Helm(&stub_rudder, &stub_compass, &stub_timer, &stub_windsensor, &stub_sail, &stub_switches, &logger);
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
  helm.steer(30, 1);
  EXPECT_EQ(rudder_position(), -NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_steer_left_towards_the_requested_heading_using_the_nudge_value) {
  uangle bearings[] = {190, 190, 190};
  stub_compass.set_bearings(bearings, 3);
  helm.steer(170, 3000);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[2], NUDGE_DEGREES);
}

TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_left) {
  uangle bearing = 0;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(170, 8000);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[0],- NUDGE_DEGREES);
  EXPECT_EQ(positions[1],- 2 * NUDGE_DEGREES);
  EXPECT_EQ(positions[15],-45);
  EXPECT_EQ(positions[16],-45);
}

 TEST_F(HelmTest, Should_not_exceed_maximum_rudder_displacement_right) {
  uangle bearing = 180;
  stub_compass.set_bearings(&bearing,1);
  helm.steer(10, 7000);
  angle *positions = stub_rudder.get_positions();
  EXPECT_EQ(positions[1], 0);
  EXPECT_EQ(positions[2], NUDGE_DEGREES);
  EXPECT_EQ(positions[15], 45);
  EXPECT_EQ(positions[16], 45);
}

 TEST_F(HelmTest, Should_steer_repeatedly_given_overall_time_and_wait_time) {
   uangle bearings[] = {0, 3, 8};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(30, 1500);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-2*NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-3*NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_steer_further_if_rudder_is_ineffective) {
   uangle bearings[] = {190,190,190};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(230, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-2 * NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-3 * NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_steer_further_only_when_not_turning) {
   uangle bearings[] = {20,15,15};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(350, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],NUDGE_DEGREES);
   EXPECT_EQ(positions[1],2 * NUDGE_DEGREES);
   EXPECT_EQ(positions[2],3 * NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_consider_very_small_changes_as_not_turning) {
   uangle bearings[] = {10,8,6};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(350, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],NUDGE_DEGREES);
   EXPECT_EQ(positions[1],2*NUDGE_DEGREES);
   EXPECT_EQ(positions[2],3*NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_ignore_single_compass_point_changes) {
   uangle bearings[] = {200,201,200};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(200, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],0);
   EXPECT_EQ(positions[1],0);
   EXPECT_EQ(positions[2],0);
 }

 TEST_F(HelmTest, Should_maintain_rudder_position_if_on_course) {
   uangle bearings[] = {210,200,200};
   stub_compass.set_bearings(bearings, 3);

   helm.steer(200, 1000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],-NUDGE_DEGREES);
   EXPECT_EQ(positions[1],-NUDGE_DEGREES);
   EXPECT_EQ(positions[2],-NUDGE_DEGREES);
 }

 TEST_F(HelmTest, Should_maintain_non_zero_rudder_position_if_approx_on_course) {
   uangle bearings[] = {100,130,152,149};
   stub_compass.set_bearings(bearings, 4);

   helm.steer(150, 2000);

   angle *positions = stub_rudder.get_positions();
   EXPECT_EQ(positions[0],4);
   EXPECT_EQ(positions[1],8);
   EXPECT_EQ(positions[2],12);
   EXPECT_EQ(positions[3],8);
   EXPECT_EQ(positions[4],8);
   EXPECT_EQ(positions[5],8);
 }

TEST_F(HelmTest, Rudder_deflection_should_be_progressive_through_turn) {
   uangle bearings[] = {200, 200, 210, 220, 230, 240, 255, 260, 275, 280, 285, 290, 300, 305, 310};
   stub_compass.set_bearings(bearings, 15);

   helm.steer(300, 7000);
   angle *positions = stub_rudder.get_positions();

   EXPECT_EQ(positions[1],-8);
   EXPECT_EQ(positions[2],-12);
   EXPECT_EQ(positions[3],-16);
   EXPECT_EQ(positions[4],-20);
   EXPECT_EQ(positions[5],-24);
   EXPECT_EQ(positions[6],-24);
   EXPECT_EQ(positions[7],-28);
   EXPECT_EQ(positions[8],-24);
   EXPECT_EQ(positions[9],-28);
   EXPECT_EQ(positions[10],-28);
   EXPECT_EQ(positions[11],-28);
   EXPECT_EQ(positions[12],-28);
   EXPECT_EQ(positions[13],-24);
   EXPECT_EQ(positions[14],-20);
   EXPECT_EQ(positions[15],-16);
   EXPECT_EQ(positions[16],-12);
}

TEST_F(HelmTest, Should_set_the_sail_every_time_we_steer) {
  uangle bearings[] = {210,190};
  angle wind[] = {70, 90};
  stub_compass.set_bearings(bearings, 2);
  stub_windsensor.set_relative(wind, 2);

  helm.steer(200, 750);

  angle *positions = stub_rudder.get_positions();
  angle *sail_calls = stub_sail.get_calls();

  EXPECT_EQ(positions[0],- NUDGE_DEGREES);
  EXPECT_EQ(positions[1],- NUDGE_DEGREES * 2);
  EXPECT_EQ(sail_calls[0],wind[0]);
  EXPECT_EQ(sail_calls[1],wind[1]);
}

TEST_F(HelmTest, Should_converge_on_the_desired_heading) {
  uangle current_heading = 30;
  uangle desired_heading = 80;
  uangle tolerance = 5;
  int i=0;

  stub_compass.set_bearings(&current_heading, 1);

  while (i < 30 && abs1(current_heading - desired_heading) > tolerance) {
    helm.steer(desired_heading, STEER_INTERVAL - 10); // steer once if less than STEER_INTERVAL
    current_heading = current_heading  - (stub_rudder.get_positions()[i]/2);
    stub_compass.set_bearings(&current_heading, 1);
    i++;
  }

  EXPECT_LT(abs1(current_heading - desired_heading), tolerance);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
