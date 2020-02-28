#include "Tacker.h"
#include "gtest/gtest.h"

namespace {

Helm stub_helm;
Compass stub_compass;
WindSensor stub_windsensor;
Tacker tacker(&stub_helm, &stub_compass, &stub_windsensor);

class TackerTest : public ::testing::Test {
 protected:
  TackerTest() {}

  void SetUp() override {
  }

};

TEST_F(TackerTest, Slightly_strange_testing_of_absolute_wind_direction_using_mock) {
  angle wind_direction = 40;
  stub_windsensor.set_relative(&wind_direction, 1);
  EXPECT_EQ(stub_windsensor.absolute(40), 80);
  EXPECT_EQ(stub_windsensor.absolute(10), 50);
  EXPECT_EQ(stub_windsensor.absolute(100), 140);
  EXPECT_EQ(stub_windsensor.absolute(245), 285);
  EXPECT_EQ(stub_windsensor.absolute(330), 10);

  wind_direction = -170;
  EXPECT_EQ(stub_windsensor.absolute(40), 230);
  EXPECT_EQ(stub_windsensor.absolute(10), 200);
  EXPECT_EQ(stub_windsensor.absolute(100), 290);
  EXPECT_EQ(stub_windsensor.absolute(245), 75);
  EXPECT_EQ(stub_windsensor.absolute(330), 160);

}


TEST_F(TackerTest, Should_steer_directly_if_out_of_no_go_region) {
  angle wind_relative = 90; stub_windsensor.set_relative(&wind_relative, 1);
  uangle bearing = 0; stub_compass.set_bearings(&bearing,1);

  tacker.steer(20, 2000, 100);

  EXPECT_EQ(stub_helm.steering(), 20);
  EXPECT_EQ(stub_helm.steer_time(), 2000);
  EXPECT_EQ(stub_helm.interval(), 100);
}


// (introduce the absolute wind direction)
// tack if the requested course is in the no-go zone
// choose the longer tack first

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
