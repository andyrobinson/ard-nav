#include "gtest/gtest.h"
#include "Boat.h"
#include "StubLogger.h"
#include "Position.h"
#include "Globe.h"

using namespace Position;

namespace {

    position kynance_cove = {49.97480, -5.23198, 5.0};
    StubLogger logger;
    Boat boat(&kynance_cove, &logger);
    Globe globe;

    class BoatTest : public ::testing::Test {
     protected:
      BoatTest() {
      }

      void SetUp() override {
      }

    };

    TEST_F(BoatTest, Should_start_at_kynance_cove) {
      EXPECT_EQ(boat.location(), kynance_cove);
    }

    TEST_F(BoatTest, Should_move) {
      boat.move(1000);
      position expected_position = globe.new_position(&kynance_cove, 0, 1.0);
      EXPECT_EQ(boat.location(), expected_position);
    }


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
