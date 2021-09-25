#include "gtest/gtest.h"
#include "Boat.h"
#include "Position.h"
#include "Globe.h"
#include "Utility.h"

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
      EXPECT_EQ(boat.location(), expected_position);
    }

    TEST_F(BoatTest, Should_change_heading_based_on_rudder) {
      Boat boat(&kynance_cove);
      boat.rudder = 20;
      boat.move(1000);
      uangle expected_heading = uadd(STARTING_HEADING,rudder_effect(boat.rudder));
      position expected_position = globe.new_position(&kynance_cove, expected_heading, 1.0);
      EXPECT_EQ(boat.location(), expected_position);
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
      angle expected_wind = add(start_wind, -rudder_effect(boat.rudder));
      std::cout << expected_wind << "\n";
      EXPECT_EQ(boat.relative_wind(), expected_wind);

      boat.rudder = 20; boat.move(1000);
      expected_wind = add(expected_wind, -rudder_effect(boat.rudder));
      std::cout << expected_wind << "\n";
      EXPECT_EQ(boat.relative_wind(), expected_wind);

      boat.rudder = 20; boat.move(1000);
      expected_wind = add(expected_wind, -rudder_effect(boat.rudder));
      std::cout << expected_wind << "\n";
      EXPECT_EQ(boat.relative_wind(), expected_wind);
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
