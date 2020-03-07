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
    stub_gps.set_data(gps_current);
    navigator.sailto(current);
    EXPECT_EQ(stub_tacker.never_called(),true);
  }

  TEST_F(NavigatorTest, Should_steer_towards_destination) {
    gpsResult gps_current = {{-20.0, -10.2, 3.0}, 1, 0.6, 12345675};
    position destination = {-21.0, -10.2, 5.0};
    stub_gps.set_data(gps_current);
    navigator.sailto(destination);
    EXPECT_EQ(stub_tacker.steering(0),180);
  }


  TEST_F(NavigatorTest, Should_steer_for_time_based_on_an_estimated_half_remaining_distance) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 1.0, 12345675};
    position destination = {10.01, 10.01, 5.0};
    double dist = globe.distance_between(&gps_current.pos, &destination);
    long expected_time = round((dist * 1000)/2.0);

    stub_gps.set_data(gps_current);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),45);
    EXPECT_EQ(stub_tacker.steer_time(0), expected_time);
  }

  TEST_F(NavigatorTest, Should_have_a_limit_of_one_hour_on_steer_time) {
    gpsResult gps_current = {{10.0, 10.0, 3.0}, 1, 0.1, 12345675};
    position destination = {30.0, -10.0, 5.0};
    double dist = globe.distance_between(&gps_current.pos, &destination);

    stub_gps.set_data(gps_current);
    navigator.sailto(destination);

    EXPECT_EQ(stub_tacker.steering(0),320);
    EXPECT_EQ(stub_tacker.steer_time(0), 3600000);
  }

  // should have a minimum time to steer
  // should steer repeatedly until the destination is reached
  // should do something if there's no GPS fix (oops)
  // approximate arrival within tolerance

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
