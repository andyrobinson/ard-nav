#include "gtest/gtest.h"
#include "Boat.h"
#include "StubLogger.h"
#include "Position.h"

using namespace Position;

namespace {

    position kynance_cove = {49.97480, -5.23198, 5.0};
    StubLogger logger;
    Boat boat(&kynance_cove, &logger);

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



}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
