#include "gtest/gtest.h"
#include "Boat.h"
#include "Position.h"
#include "Globe.h"
#include "Utility.h"
#include "math.h"
#include <iostream>

using namespace Position;
using namespace Utility;

namespace {

    position kynance_cove = {49.97480, -5.23198, 5.0};
    Globe globe;

    class BoatTest : public ::testing::Test {
     protected:
      BoatTest() {
      }

      void SetUp() override {
      }

      double percentage_diff(double x, double y) {
        double diff = (100.0 * (x-y))/x;
        return fabs(diff);
      }


    angle rudder_effect(angle deflection) {
      return -deflection;
    }

    };

    TEST_F(BoatTest, Should_start_at_provided_location) {
      Boat boat(&kynance_cove);
      EXPECT_EQ(boat.location(), kynance_cove);
    }

    TEST_F(BoatTest, Should_move_in_initial_direction) {
      Boat boat(&kynance_cove);
      boat.move(1000);
      position expected_position = globe.new_position(&kynance_cove, STARTING_HEADING, 1.0);
      EXPECT_LT(percentage_diff(boat.location().latitude, expected_position.latitude),0.1);
      EXPECT_LT(percentage_diff(boat.location().longitude, expected_position.longitude),0.1);
    }

    TEST_F(BoatTest, Should_change_heading_based_on_rudder) {
      Boat boat(&kynance_cove, 1.0);
      boat.rudder=20;
      boat.move(1000);
      uangle expected_heading = uadd(STARTING_HEADING,rudder_effect(20));
      EXPECT_EQ(boat.heading,expected_heading);
    }

    TEST_F(BoatTest, Should_report_stats) {
      Boat boat(&kynance_cove);
      EXPECT_EQ(boat.speed(),STARTING_SPEED);
      EXPECT_EQ(boat.bearing(),STARTING_HEADING);
    }

    TEST_F(BoatTest, Should_return_relative_wind) {
      Boat boat(&kynance_cove);
      angle start_wind = add(STARTING_WIND, -STARTING_HEADING);
      EXPECT_EQ(boat.relative_wind(), start_wind);

      boat.rudder = -30; boat.move(1000);
      angle expected_wind = add(STARTING_WIND, -boat.bearing());
      EXPECT_EQ(boat.relative_wind(), expected_wind);

    }

    TEST_F(BoatTest, Drag_should_increase_with_speed) {
      Boat boat(&kynance_cove);
      EXPECT_LT(percentage_diff(boat.drag(0.2),0.0928571),0.1);
      EXPECT_LT(percentage_diff(boat.drag(0.4),0.452174),0.1);
    }

    TEST_F(BoatTest, Drag_should_increase_with_massively_near_hull_speed) {
      Boat boat(&kynance_cove);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS - 0.2), 8.125),0.1);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS - 0.1), 14.3),0.1);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS - 0.01), 28.322),0.1);
    }

    TEST_F(BoatTest, Drag_should_never_overflow_or_go_negative) {
      Boat boat(&kynance_cove);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS - 0.01), 28.322),0.1);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS), 31.2),0.1);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS + 0.01), 34.606),0.1);
      EXPECT_LT(percentage_diff(boat.drag(HULL_SPEED_MS * 2), 12480),0.1);
    }

    TEST_F(BoatTest, Speed_should_increase_if_impetus_is_greater_than_drag) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 4.34, 3.0, 1000); // 1.34 newtons should give 0.1 mss
      EXPECT_LT(percentage_diff(result, 0.6), 0.1);
    }

    TEST_F(BoatTest, Speed_should_increase_according_to_interval) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 4.34, 3.0, 2000); // 1.34 newtons should give 0.1 mss
      EXPECT_LT(percentage_diff(result, 0.7), 0.1);
    }

    TEST_F(BoatTest, Speed_should_decrease_if_drag_greater_than_impetus) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 3.0, 4.34, 1000); // 1.34 newtons should give 0.1 mss
      EXPECT_LT(percentage_diff(result, 0.4), 0.1);
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
