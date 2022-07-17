#include "Tacker.h"
#include "gtest/gtest.h"
#include "math.h"
#include "StubLogger.h"
#include "Helm.h"

namespace {

Helm stub_helm;
Compass stub_compass;
WindSensor stub_windsensor;
StubLogger logger;
Tacker tacker(&stub_helm, &stub_compass, &stub_windsensor, &logger);

class TackerTest : public ::testing::Test {
 protected:
  TackerTest() {}

  void SetUp() override {
    stub_helm.reset();
  }

};

TEST_F(TackerTest, Slightly_strange_testing_of_absolute_wind_direction_using_mock) {
  angle wind_direction = 40;
  stub_windsensor.set_relative(&wind_direction, 1);
  EXPECT_EQ(stub_windsensor.absolute(40), 80);
  EXPECT_EQ(stub_windsensor.absolute(10), 50);
  EXPECT_EQ(stub_windsensor.absolute(100), 140);
  EXPECT_EQ(stub_windsensor.absolute(245), 285);
  EXPECT_EQ(stub_windsensor.absolute(330), 10);

  wind_direction = -170;
  EXPECT_EQ(stub_windsensor.absolute(40), 230);
  EXPECT_EQ(stub_windsensor.absolute(10), 200);
  EXPECT_EQ(stub_windsensor.absolute(100), 290);
  EXPECT_EQ(stub_windsensor.absolute(245), 75);
  EXPECT_EQ(stub_windsensor.absolute(330), 160);

}

TEST_F(TackerTest, Should_steer_directly_if_out_of_no_go_region) {
  angle wind_relative = 90; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 0; stub_compass.set_bearings(&bearing,1);

  tacker.steer(20, 2000);

  EXPECT_EQ(stub_helm.steering(0), 20);
  EXPECT_EQ(stub_helm.steer_time(0), 2000);
}

TEST_F(TackerTest, Wind_range_should_be_nogo_if_out_of_no_go_region) {
  angle wind_relative = 90; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 0; stub_compass.set_bearings(&bearing,1);

  tacker.steer(20, 2000);

  EXPECT_EQ(stub_helm.wind_range(0).lower, uadd(20,WIND_RANGE_NO_GO_LIMIT));
  EXPECT_EQ(stub_helm.wind_range(0).upper, uadd(20,-WIND_RANGE_NO_GO_LIMIT));
}

TEST_F(TackerTest, Wind_range_should_be_starboard_no_go_and_180_if_tacking_left) {
  angle wind_relative = TACKER_NO_GO_LIMIT-5; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 0; stub_compass.set_bearings(&bearing,1);
  uangle expectedDirection = 355;

  tacker.steer(0, 2000);

  EXPECT_EQ(stub_helm.steering(0), expectedDirection);
  EXPECT_EQ(stub_helm.wind_range(0).lower, uadd(expectedDirection,WIND_RANGE_NO_GO_LIMIT));
  EXPECT_EQ(stub_helm.wind_range(0).upper, uadd(expectedDirection,180));
}

TEST_F(TackerTest, Should_tack_right_with_correct_wind_range_if_in_nogo_and_closest_to_desired_course) {
  angle wind_relative = 5-TACKER_NO_GO_LIMIT; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 180; stub_compass.set_bearings(&bearing,1);
  uangle expectedDirection = 185;

  tacker.steer(180, 2000);

  EXPECT_EQ(stub_helm.steering(0), expectedDirection);
  EXPECT_EQ(stub_helm.wind_range(0).lower, uadd(expectedDirection,180));
  EXPECT_EQ(stub_helm.wind_range(0).upper, uadd(expectedDirection,-WIND_RANGE_NO_GO_LIMIT));
}

TEST_F(TackerTest, Should_tack_left_if_in_nogo_and_only_just_to_left_of_wind) {
  angle wind_relative = 2; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 270; stub_compass.set_bearings(&bearing,1);

  tacker.steer(270, 2000);

  EXPECT_EQ(stub_helm.steering(0), 232);
}

TEST_F(TackerTest, Should_adjust_tack_time_using_COS_of_angle_for_first_tack) {
  angle wind_relative = 20; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 95; stub_compass.set_bearings(&bearing,1);

  tacker.steer(95, 2000);

  EXPECT_EQ(stub_helm.steering(0), 75);
  EXPECT_EQ(stub_helm.steer_time(0), round (2000 * cos(to_radians (TACKER_NO_GO_LIMIT - wind_relative))));
}

TEST_F(TackerTest, Should_adjust_tack_time_using_SIN_of_angle_for_second_tack) {
  angle wind_relative = 20; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 80; stub_compass.set_bearings(&bearing,1);

  tacker.steer(80, 20000);

  EXPECT_EQ(stub_helm.steering(1), 140);
  EXPECT_EQ(stub_helm.steer_time(1), round (20000 * sin(to_radians (TACKER_NO_GO_LIMIT - wind_relative))));
}

TEST_F(TackerTest, Should_correctly_set_wind_range_for_second_tack) {
  angle wind_relative = 20; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 80; stub_compass.set_bearings(&bearing,1);
  uangle expectedDirection = 140;

  tacker.steer(80, 20000);

  EXPECT_EQ(stub_helm.steering(1), expectedDirection);
  EXPECT_EQ(stub_helm.wind_range(1).lower, uadd(expectedDirection,180));
  EXPECT_EQ(stub_helm.wind_range(1).upper, uadd(expectedDirection,-WIND_RANGE_NO_GO_LIMIT));
}

TEST_F(TackerTest, Should_not_do_second_tack_if_below_limit) {
  angle wind_relative = 5-TACKER_NO_GO_LIMIT; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 270; stub_compass.set_bearings(&bearing,1);

  tacker.steer(270, 2000);

  EXPECT_EQ(stub_helm.steering(0), 275);
  EXPECT_EQ(stub_helm.steering_calls(),1);
}

TEST_F(TackerTest, Attempt_to_steer_straight_if_compass_in_error) {
  angle wind_relative = 0; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = ANGLE_ERROR; stub_compass.set_bearings(&bearing,1);

  tacker.steer(40, 2000);

  EXPECT_EQ(stub_helm.steering(0), 40);
  EXPECT_EQ(stub_helm.steering_calls(),1);
}

TEST_F(TackerTest, Attempt_to_steer_straight_if_windsensor_in_error) {
  angle wind_relative = ANGLE_ERROR; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 0; stub_compass.set_bearings(&bearing,1);

  tacker.steer(20, 2000);

  EXPECT_EQ(stub_helm.steering(0), 20);
  EXPECT_EQ(stub_helm.steering_calls(),1);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
