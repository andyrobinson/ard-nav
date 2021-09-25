#include "WindSensor.h"
#include "Boat.h"
#include "StubLogger.h"
#include "gtest/gtest.h"

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
StubLogger logger;
Boat boat(&chorlton, &logger);
WindSensor windsensor(&boat);

class WindSensorTest : public ::testing::Test {
 protected:
  WindSensorTest() {}
};

TEST_F(WindSensorTest, should_return_wind_result_based_on_boat_value) {
  EXPECT_EQ(windsensor.relative(), boat.relative_wind());
  boat.rudder = 12;
  boat.move(1500);

  EXPECT_EQ(windsensor.relative(), boat.relative_wind());
  EXPECT_EQ(windsensor.absolute(boat.bearing()),STARTING_WIND);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
