#include "Captain.h"
#include "gtest/gtest.h"

namespace {

  Navigator stub_navigator;
  Captain captain(&stub_navigator);

  class CaptainTest : public ::testing::Test {
   protected:
    CaptainTest() {}

    void SetUp() override {
      stub_navigator.reset();
    }

  };

  TEST_F(CaptainTest, Should_be_able_to_save_destinations_in_the_mock) {
    waypoint first = {'A', {10.0, 10.0, 1.0}};
    waypoint second = {'2', {11.0, 11.0, 1.0}};
    stub_navigator.sailto(first);
    stub_navigator.sailto(second);
    waypoint *waypoints = stub_navigator.get_waypoints();

    EXPECT_EQ(waypoints[0], first);
    EXPECT_EQ(waypoints[1], second);
  }

  TEST_F(CaptainTest, Should_navigate_to_the_first_waypoint) {
    waypoint first = {'A', {-10.0, -20.0, 1.0}};
    waypoint route[] = {first};
    captain.voyage(route,1);

    waypoint *waypoints = stub_navigator.get_waypoints();
    EXPECT_EQ(waypoints[0],first);
  }

  TEST_F(CaptainTest, Should_navigate_to_each_waypoint_in_turn) {
    waypoint first = {'A', {-10.0, -20.0, 1.0}};
    waypoint second = {'B', {-12.0, -22.0, 1.0}};
    waypoint third = {'C', {-13.0, -24.0, 1.0}};
    waypoint route[] = {first,second,third};
    captain.voyage(route,3);

    waypoint *waypoints = stub_navigator.get_waypoints();

    EXPECT_EQ(waypoints[0],first);
    EXPECT_EQ(waypoints[1],second);
    EXPECT_EQ(waypoints[2],third);
  }

  // should be able to selectively bypass waypoints, but this means that we
  // need to return to the captain even if we're not finished - the Captain
  // would need to allocate a time, and then re-evaluate the navigation

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
