#include "Navigator.h"
#include "gtest/gtest.h"
#include "math.h"
#include "Utility.h"

using namespace Utility;

namespace {

  Tacker stub_tacker;
  Gps stub_gps;
  Globe globe;
  Navigator navigator(&stub_tacker, &stub_gps, &globe);

  class NavigatorTest : public ::testing::Test {
   protected:
    NavigatorTest() {}

    void SetUp() override {
      stub_tacker.reset();
    }

  };

  TEST_F(NavigatorTest, Should_return_immediately_if_we_have_already_arrived) {
    position current = {52.0, -2.2, 3.0};
    gpsResult gps_current = {current, 1, 0.6, 12345675};
    stub_gps.set_data(&gps_current,1);
    navigator.sailto(current);
    EXPECT_EQ(stub_tacker.never_called(),true);
  }

  TEST_F(NavigatorTest, Should_return_immediately_if_we_have_already_arrived_within_tolerance) {
    double tolerance = 8.0;
    position current = {52.0, -2.2, 0.0};
    position desired = {52.00005, -2.19995, tolerance};
    gpsResult gps_current = {current, 1, 0.6, 12345675};
    double dist = globe.distance_between(&gps_current.pos, &desired);
    stub_gps.set_data(&gps_current,1);

    navigator.sailto(desired);

    EXPECT_LT(dist, tolerance);
    EXPECT_EQ(stub_tacker.never_called(),true);
  }

  TEST_F(NavigatorTest, Should_steer_towards_destination) {
    gpsResult gps_current = {{-20.0, -10.2, 3.0}, 1, 0.6, 12345675};
    position destination = {-21.0, -10.2, 5.0};
    gpsResult gpsData[] = {gps_current, {destination,1,0.6,1}};
    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);
    EXPECT_EQ(stub_tacker.steering(0),180);
  }


  TEST_F(NavigatorTest, Should_steer_for_time_based_on_an_estimated_half_remaining_distance) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 1.0, 12345675};
    position destination = {10.01, 10.01, 5.0};
    gpsResult gpsData[] = {gps_current, {destination,1,0.6,1}};
    double dist = globe.distance_between(&gps_current.pos, &destination);
    long expected_time = round((dist * 1000)/2.0);

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),45);
    EXPECT_EQ(stub_tacker.steer_time(0), expected_time);
  }

  TEST_F(NavigatorTest, Should_have_a_limit_of_one_hour_on_steer_time) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 0.1, 12345675};
    position destination = {30.0, -10.0, 5.0};
    gpsResult gpsData[] = {gps_current, {destination,1,0.6,1}};

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),320);
    EXPECT_EQ(stub_tacker.steer_time(0), 3600000);
  }

  TEST_F(NavigatorTest, Should_have_a_minimum_steer_time_of_5s) {
    gpsResult gps_current = {{28.0, -16.0, 3.0}, 2, 2, 12345675};
    position destination = {27.9999, -15.9999, 5.0};
    gpsResult gpsData[] = {gps_current, {destination,1,0.6,1}};

    stub_gps.set_data(gpsData,2);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),139);
    EXPECT_EQ(stub_tacker.steer_time(0), 5000);
  }

  TEST_F(NavigatorTest, Should_steer_repeatedly_until_reached_destination) {
    gpsResult gps_current = {{30.0, 30.0, 3.0}, 2, 2000.0, 12345675};
    gpsResult gps_midpoint = {{29.5, 29.5, 3.0}, 2, 2000.0, 12345675};
    position destination = {29.0, 29.0, 5.0};
    gpsResult gpsData[] = {gps_current, gps_midpoint, {destination,1,0.6,1}};

    stub_gps.set_data(gpsData,3);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),221);
    EXPECT_EQ(stub_tacker.steer_time(0), 36853);
    EXPECT_EQ(stub_tacker.steering(1),221);
    EXPECT_EQ(stub_tacker.steer_time(1), 18446);
  }
  // should do something if there's no GPS fix; need some kind of dead reckoning

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
