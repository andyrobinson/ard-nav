#include "gtest/gtest.h"
#include "Boat.h"
#include "Position.h"
#include "Globe.h"
#include "Utility.h"
#include "math.h"

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
      return 90 - deflection;
    }

    void set_rudder(Boat *boat, angle deflection) {
        boat->rudder = 90-deflection;
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
      Boat boat(&kynance_cove);
      set_rudder(&boat, 20);
      boat.move(1000);
      uangle expected_heading = uadd(STARTING_HEADING,rudder_effect(20));
      position expected_position = globe.new_position(&kynance_cove, expected_heading, 1.0);
      EXPECT_LT(percentage_diff(boat.location().latitude, expected_position.latitude),0.1);
      EXPECT_LT(percentage_diff(boat.location().longitude, expected_position.longitude),0.1);
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

      set_rudder(&boat,-30); boat.move(1000);
      angle expected_wind = add(STARTING_WIND, -boat.bearing());
      EXPECT_EQ(boat.relative_wind(), expected_wind);

    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
