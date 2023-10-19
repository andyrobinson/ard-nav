#include "gtest/gtest.h"
#include "Boat.h"
#include "Position.h"
#include "Globe.h"
#include "Utility.h"
#include "math.h"
#include <iostream>
//#include <cmath>

using namespace Position;
using namespace Utility;

namespace {

    position kynance_cove = {49.97480, -5.23198, 5.0};
    Globe globe;

    class BoatTest : public ::testing::Test {
     protected:
      BoatTest() {
      }

      double roundto(double value, int places) {
        return ((double) round(value * pow(10, places)))/pow(10, places);
      }
      void SetUp() override {
      }

    angle rudder_effect(angle deflection) {
      return -deflection;
    }

    };

    TEST_F(BoatTest, Should_start_at_provided_location) {
      Boat boat(&kynance_cove);
      EXPECT_EQ(boat.location(), kynance_cove);
    }

    TEST_F(BoatTest, Should_move_in_initial_direction_about_2m) {
      Boat boat(&kynance_cove, 1.0);
      boat.move(2000);
      position expected_position = globe.new_position(&kynance_cove, STARTING_HEADING, 2.0);
      EXPECT_NEAR(boat.location().latitude, expected_position.latitude,0.00001);
      EXPECT_NEAR(boat.location().longitude, expected_position.longitude, 0.00001);
    }

    TEST_F(BoatTest, Should_change_heading_based_on_rudder) {
      Boat boat(&kynance_cove, 1.0);
      int rudder_deflection = 20;
      boat.rudder=90 + rudder_deflection;
      boat.move(1000);
      uangle expected_heading = uadd(STARTING_HEADING,rudder_effect(rudder_deflection));
      EXPECT_NEAR(boat.heading,expected_heading,1);
    }

    TEST_F(BoatTest, Should_report_stats) {
      Boat boat(&kynance_cove);
      EXPECT_EQ(boat.speed(),STARTING_SPEED);
      EXPECT_EQ(boat.bearing(),STARTING_HEADING);
    }

    TEST_F(BoatTest, Should_return_relative_wind) {
      Boat boat(&kynance_cove);
      angle start_wind = add(STARTING_WIND_DIRECTION, -STARTING_HEADING);
      EXPECT_EQ(boat.relative_wind(), start_wind);

      boat.rudder = -30; boat.move(1000);
      angle expected_wind = add(STARTING_WIND_DIRECTION, -boat.bearing());
      EXPECT_EQ(boat.relative_wind(), expected_wind);
    }

    TEST_F(BoatTest, Drag_should_increase_with_speed) {
      Boat boat(&kynance_cove);
      EXPECT_NEAR(boat.hull_drag(0.2),0.092857,0.000001);
      EXPECT_NEAR(boat.hull_drag(0.4),0.452174,0.000001);
    }

    TEST_F(BoatTest, Drag_should_increase_with_massively_near_hull_speed) {
      Boat boat(&kynance_cove);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS - 0.2), 8.125,0.0001);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS - 0.1), 14.3,0.0001);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS - 0.01), 28.322,0.0001);
    }

    TEST_F(BoatTest, Drag_should_never_overflow_or_go_negative) {
      Boat boat(&kynance_cove);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS - 0.01), 28.322,0.0001);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS), 31.2,0.0001);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS + 0.01), 34.606,0.0001);
      EXPECT_NEAR(boat.hull_drag(HULL_SPEED_MS * 2), 12480,0.0001);
    }

    TEST_F(BoatTest, Speed_should_increase_if_impetus_is_greater_than_drag) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 4.34, 3.0, 1000); // 1.34 newtons should give 0.1 mss
      ASSERT_DOUBLE_EQ(result, 0.6);
    }

    TEST_F(BoatTest, Speed_should_increase_according_to_interval) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 4.34, 3.0, 2000); // 1.34 newtons should give 0.1 mss
      ASSERT_DOUBLE_EQ(result, 0.7);
    }

    TEST_F(BoatTest, Speed_should_decrease_if_drag_greater_than_impetus) {
      Boat boat(&kynance_cove);
      double speed = 0.5;
      double result = boat.new_speed(speed, 3.0, 4.34, 1000); // 1.34 newtons should give 0.1 mss
      ASSERT_DOUBLE_EQ(result, 0.4);
    }

    // TEST_F(BoatTest, Sail_force_should_vary_with_wind_maximum_on_broad_reach) {
    //   Boat boat(&kynance_cove);
    //   boat.heading = 0;
    //   int sail_positions[] = {180,172,165,157,150,150,150,150,150,150,140,130,120,110,100,90,90,90,90,90,90,90,80,70,60,50,40,30,30,30,30,30,30,23,15,8,0};
    //   double expected_force[] = {12.1078,12.2165,12.1106,12.0551,11.9108,12.2490,12.9782,13.7714,14.3242,14.3815,13.6507,12.5050,10.9795,9.1203,6.984,
    //   4.6354,2.4575,-0.8062,-1.806,-0.8062,2.4575,4.6354,6.9839,9.1202,10.9794,12.5050,13.6507,14.3815,14.3242,13.7714,12.9782,12.2490,11.9108,12.0551,
    //   12.1106,12.2165,12.1078};
    //   for (int i=-180; i <=180; i+=10) {
    //     int index =i/10 + 18;
    //     boat.absolute_wind = i;
    //     boat.sail = sail_positions[index]; // sail normally set by Sail class
    //     ASSERT_DOUBLE_EQ(roundto(boat.sail_force(),4),expected_force[index]);
    //   }
    // }

    TEST_F(BoatTest, Heel_angle_should_increase_with_wind_force_but_never_reach_90) {
      Boat boat(&kynance_cove);
      double expected_heel[] = {0.0,24.4762,42.3162,53.7869,61.2253,66.282,69.8913,72.5773,74.64560, 76.2835};
      for (int i=0; i<10; i++) {
        ASSERT_DOUBLE_EQ(roundto(boat.heel((double) i),4),expected_heel[i]);
      }
      ASSERT_DOUBLE_EQ(roundto(boat.heel(100.0),4),88.7416);
    }

    TEST_F(BoatTest, Heel_angle_should_vary_with_point_of_sail_max_at_close_hauled) {
      Boat boat(&kynance_cove);
      boat.heading = 0;
      boat.wind_speed = 5.0;
      int sail_positions[] = {180,172,165,157,150,150,150,150,150,150,140,130,120,110,100,90,90,90,90,90,90,90,80,70,60,50,40,30,30,30,30,30,30,23,15,8,0};
      double expected_heel[] = {0.0,0.0,0.0};
      for (int i=-180; i <=180; i+=10) {
        int index =i/10 + 18;
        boat.absolute_wind = i;
        boat.sail_force(); // called to set the heel angle
        boat.sail = sail_positions[index]; // sail normally set by Sail class
        //ASSERT_DOUBLE_EQ(roundto(boat.heel_angle,4),roundto(expected_heel[index],4));
      }
    }

    TEST_F(BoatTest, Useful_sail_force_should_reach_maximum_then_decrease_because_of_heel) {
      GTEST_SKIP();
    }

    TEST_F(BoatTest, Rudder_effectiveness_should_decrease_with_heel) {
      GTEST_SKIP();
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
