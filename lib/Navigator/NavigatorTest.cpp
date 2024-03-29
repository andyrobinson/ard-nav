#include "Navigator.h"
#include "gtest/gtest.h"
#include "math.h"
#include "Utility.h"
#include "StubLogger.h"
#include "SatComm.h"

using namespace Utility;

namespace {

  Tacker stub_tacker;
  Gps stub_gps;
  Globe globe;
  StubLogger logger;
  SatComm satcomm;
  Navigator navigator(&stub_tacker, &stub_gps, &globe, &satcomm, &logger);

  class NavigatorTest : public ::testing::Test {
   protected:
    NavigatorTest() {}

    void SetUp() override {
      stub_tacker.reset();
    }

  };

  TEST_F(NavigatorTest, Should_return_immediately_if_we_have_already_arrived) {
    waypoint current = {'d',{52.0, -2.2, 3.0}};
    gpsResult gps_current = {current.pos, 1, 0.6, 0.6, 120, 12345675, 5200000,-220000};
    stub_gps.set_data(&gps_current,1);
    navigator.sailto(current);
    EXPECT_EQ(stub_tacker.never_called(),true);
  }

  TEST_F(NavigatorTest, Should_return_immediately_if_we_have_already_arrived_within_tolerance) {
    double tolerance = 8.0;
    position current = {52.0, -2.2, 0.0};
    waypoint desired = {'d', {52.00005, -2.19995, tolerance}};
    gpsResult gps_current = {current, 1, 0.6, 0.6, 120, 12345675, 5200000,-220000};
    double dist = globe.distance_between(&current, &desired.pos);
    stub_gps.set_data(&gps_current,1);

    navigator.sailto(desired);

    EXPECT_LT(dist, tolerance);
    EXPECT_EQ(stub_tacker.never_called(),true);
  }

  TEST_F(NavigatorTest, Should_steer_towards_destination) {
    gpsResult gps_current = {{-20.0, -10.2, 3.0}, 1, 0.6, 0.6, 120, 12345675, -2000000,-1020000};
    waypoint destination = {'d', {-21.0, -10.2, 5.0}};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.6,0.6,120, 1, -2100000, -1020000}};
    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);
    EXPECT_EQ(stub_tacker.steering(0),180);
  }

  TEST_F(NavigatorTest, Should_steer_for_time_based_on_an_estimated_half_remaining_distance_using_average_speed) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 0.3, 1.0, 120, 12345675, 1000000, 1000000};
    waypoint destination = {'d', {10.0002, 10.0002, 5.0}};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.3,0.6,120, 1, 1000020, 1000020}};
    double dist = globe.distance_between(&gps_current.pos, &destination.pos);
    long expected_time = round((dist * 1000)/2.0);

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),45);
    EXPECT_EQ(stub_tacker.steer_time(0), expected_time);
  }

  TEST_F(NavigatorTest, Should_have_a_limit_on_steer_time) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 1.0, 0.1, 120, 12345675, 1000000, 1000000};
    waypoint destination = {'e', {30.0, -10.0, 5.0}};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.6, 0.6,120, 1, 3000000, -1000000}};

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),320);
    EXPECT_EQ(stub_tacker.steer_time(0), MAX_STEER_TIME);
  }

  TEST_F(NavigatorTest, Should_not_overflow_even_if_slow_speed_and_large_distances) {
    position Moscow = {55.755826, 37.6173,0.0};
    position Santiago = {-33.4691199,-70.641997,5.0};

    gpsResult gps_current = {Moscow, 1, 1.0, 0.01, 120, 12345675, 5575582, 3761730};
    waypoint destination = {'e', Santiago};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.6, 0.6,120, 1, -3346912,-7064100}};

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steer_time(0), MAX_STEER_TIME);
  }

  TEST_F(NavigatorTest, Should_have_a_minimum_steer_time_of_10s) {
    gpsResult gps_current = {{28.0, -16.0, 3.0}, 2, 2, 2, 120, 12345675, 2800000, -1600000};
    waypoint destination = {'e', {27.9999, -15.9999, 5.0}};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.6, 0.6,120, 1, 2799990, 1599990}};

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),139);
    EXPECT_EQ(stub_tacker.steer_time(0), 10000);
  }

  TEST_F(NavigatorTest, Should_steer_repeatedly_until_reached_destination) {
    gpsResult gps_current = {{30.0, 30.0, 3.0}, 2, 1.0, 3500.0, 120, 12345675, 3000000, 3000000};
    gpsResult gps_midpoint = {{29.5, 29.5, 3.0}, 2, 1.0, 2500.0, 120, 12345675, 2950000, 2950000};
    waypoint destination = {'e', {29.0, 29.0, 5.0}};
    gpsResult gpsData[] = {gps_current, gps_midpoint, {destination.pos,1,0.6,120, 1, 2900000, 2900000}};

    stub_gps.set_data(gpsData,3);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),221);
    EXPECT_EQ(stub_tacker.steer_time(0), 21059);
    EXPECT_EQ(stub_tacker.steering(1),221);
    EXPECT_EQ(stub_tacker.steer_time(1), 14757);
  }

  TEST_F(NavigatorTest, Should_set_waypoint_in_logger_and_satcomm) {
    gpsResult gps_current = {{-20.0, -10.2, 3.0}, 1, 0.6, 0.6, 120, 12345675, -2000000,-1020000};
    waypoint destination = {'x', {-21.0, -10.2, 5.0}};
    gpsResult gpsData[] = {gps_current, {destination.pos,1,0.6,0.6,120, 1, -2100000, -1020000}};
    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(logger.waypoint, 'x');
    EXPECT_EQ(satcomm.waypoint,'x');
  }

  // should do something if there's no GPS fix; need some kind of dead reckoning

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
