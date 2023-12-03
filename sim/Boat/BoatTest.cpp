#include "gtest/gtest.h"
#include "Boat.h"
#include "Position.h"
#include "Globe.h"
#include "Utility.h"
#include "MServo.h"
#include "Sail.h"
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

    double sail_force(angle wind, Sail *sail, Boat *boat) {
        boat->absolute_wind = wind;
        sail->set_position(boat->relative_wind());
        return roundto(boat->sail_force(),4);
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
      EXPECT_NEAR(boat.heading,expected_heading,2);
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

    TEST_F(BoatTest, Drag_should_increase_massively_near_hull_speed) {
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

    TEST_F(BoatTest, Sail_force_should_be_symmetrical) {
      Boat boat(&kynance_cove);
      MServo mservo(&boat);
      Sail sail(&mservo);
      boat.heading = 0;
      boat.wind_speed = 5.0;
      for (int i=0; i <=180; i+=10) {
        double force = sail_force(i,&sail,&boat);
        double minus_force = sail_force(-i,&sail,&boat);
        EXPECT_NEAR(abs1(force),abs1(minus_force),0.001);
      }
    }

    TEST_F(BoatTest, Sail_force_should_be_maximum_on_broad_reach) {
      Boat boat(&kynance_cove);
      MServo mservo(&boat);
      Sail sail(&mservo);
      boat.heading = 0;
      boat.wind_speed = 5.0;
      double old_force, force, max_force;
      old_force = 0.0;
      for (int i=0; i <=180; i+=10) {
        force = sail_force(i,&sail,&boat);
        if (i == 90) max_force = force;
        if (i > 40) EXPECT_GT(force, 1.0);
        if (i > 0 && i <= 90) EXPECT_GT(force,old_force);
        if (i > 90) EXPECT_GT(max_force, force);
        old_force = force;
      }
    }

    TEST_F(BoatTest, Heel_angle_should_increase_with_wind_force_but_never_reach_90) {
      Boat boat(&kynance_cove);
      double heel = 0.0;
      for (int i=1; i<10; i++) {
        double new_heel = boat.heel((double) i);
        ASSERT_GT(new_heel,heel);
        ASSERT_LT(new_heel, 90.0);
        heel = new_heel;
      }
      ASSERT_GT(boat.heel(100.0),85.0);
    }

    TEST_F(BoatTest, Heel_angle_should_vary_with_point_of_sail_max_at_close_hauled) {
      Boat boat(&kynance_cove);
      MServo mservo(&boat);
      Sail sail(&mservo);
      boat.heading = 0;
      boat.wind_speed = 5.0;
      double max_heel = 0.0;
      int max_heel_wind = 0;
      for (int i=-180; i <=180; i+=10) {
        sail_force(i,&sail,&boat); // calculates heel
        if (abs1(boat.heel_angle) > max_heel) {
          max_heel = abs1(boat.heel_angle);
          max_heel_wind = abs1(i);
        }
      }
      ASSERT_EQ(max_heel_wind,30);
    }

    TEST_F(BoatTest, Useful_sail_force_should_reach_maximum_then_decrease_because_of_heel) {
      Boat boat(&kynance_cove);
      MServo mservo(&boat);
      Sail sail(&mservo);
      double force,max_force = 0.0;
      int max_wind = 0;
      for (int i=1; i<200; i++) {
        boat.wind_speed = (double) i;
        force = sail_force(60,&sail,&boat); 
        std::cout << force << ", " << boat.heel_angle << ", " << i << "\n";
        if (force > max_force) {
          max_force = force;
          max_wind = i;
        }
      }
      ASSERT_EQ(max_wind, 50);
    }

    TEST_F(BoatTest, Rudder_effectiveness_should_decrease_with_heel) {
      GTEST_SKIP();
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
