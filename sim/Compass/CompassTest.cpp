#include "Compass.h"
#include "Boat.h"
#include "gtest/gtest.h"

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
Boat boat(&chorlton);
Compass compass(&boat);

class CompassTest : public ::testing::Test {
 protected:
  CompassTest() {}
};

TEST_F(CompassTest, should_return_result_based_on_boat) {
  EXPECT_EQ(compass.bearing(),boat.bearing());
  boat.rudder = -20;
  boat.move(1000);
  EXPECT_EQ(compass.bearing(),boat.bearing());
}

TEST_F(CompassTest, should_return_zero_errors) {
  EXPECT_EQ(compass.err_percent(), 0);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
