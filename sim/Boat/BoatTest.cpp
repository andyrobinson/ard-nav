#include "gtest/gtest.h"
#include "Boat.h"
#include "StubLogger.h"

namespace {

    StubLogger logger;
    Boat boat(&logger);

    class BoatTest : public ::testing::Test {
     protected:
      BoatTest() {
      }

      void SetUp() override {
      }

    };

    TEST_F(BoatTest, Should_start_at_kynance_cove) {
      position expectedPosition = {49.97480, -5.23198, 5.0};
      EXPECT_EQ(boat.location(), expectedPosition);
    }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
