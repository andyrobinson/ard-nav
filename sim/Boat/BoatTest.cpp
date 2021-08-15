#include "gtest/gtest.h"
#include "Boat.h"
#include "StubLogger.h"
#include "Position.h"
#include "Globe.h"

using namespace Position;

namespace {

    position kynance_cove = {49.97480, -5.23198, 5.0};
    StubLogger logger;
    Globe globe;

    class BoatTest : public ::testing::Test {
     protected:
      BoatTest() {
      }

      void SetUp() override {
      }

    };

    TEST_F(BoatTest, Should_start_at_provided_location) {
      Boat boat(&kynance_cove, &logger);
      EXPECT_EQ(boat.location(), kynance_cove);
    }

    TEST_F(BoatTest, Should_move_north) {
      Boat boat(&kynance_cove, &logger);
      boat.move(1000);
      position expected_position = globe.new_position(&kynance_cove, 0, 1.0);
      EXPECT_EQ(boat.location(), expected_position);
    }

    TEST_F(BoatTest, Should_change_heading_based_on_rudder) {
      Boat boat(&kynance_cove, &logger);
      boat.rudder = 20;
      boat.move(1000);
      uangle expected_heading = uadd(0,-boat.rudder);
      position expected_position = globe.new_position(&kynance_cove, expected_heading, 1.0);
      std::cout << "heading: " << boat.heading << "\n";
      EXPECT_EQ(boat.location(), expected_position);
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
