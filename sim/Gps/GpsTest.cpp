#include "Gps.h"
#include "Boat.h"
#include "gtest/gtest.h"

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
Boat boat(&chorlton);
Timer timer(&boat);
Gps gps(&boat,&timer);

class GpsTest : public ::testing::Test {
 protected:
  GpsTest() {}
};

TEST_F(GpsTest, should_return_gps_result_based_on_boat_data) {
  gpsResult gpsresult;
  gps.data(100, &gpsresult);
  float expected_avg_speed = (0.9 * MIN_SPEED + 0.1 * boat.speed());
  long expectedTimeInSeconds = 1234;

  EXPECT_EQ(gpsresult.pos.latitude, chorlton.latitude);
  EXPECT_EQ(gpsresult.pos.longitude, chorlton.longitude);
  EXPECT_EQ(gpsresult.fpLongitude, (int32_t) chorlton.longitude * 100000);
  EXPECT_EQ(gpsresult.fpLatitude, (int32_t) chorlton.latitude * 100000);
  EXPECT_EQ(gpsresult.fix, FIX_DGPS);
  EXPECT_EQ(gpsresult.mps, boat.speed());
  EXPECT_EQ(gpsresult.avg_mps, expected_avg_speed);
  EXPECT_EQ(gpsresult.cog, boat.heading);

  timer.wait(expectedTimeInSeconds * 1000);
  gps.data(100, &gpsresult);
  EXPECT_EQ(gpsresult.unixTime, expectedTimeInSeconds);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
