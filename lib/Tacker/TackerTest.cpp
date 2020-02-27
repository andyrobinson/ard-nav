#include "Tacker.h"
#include "gtest/gtest.h"

namespace {

Helm stub_helm;
Compass stub_compass;
Timer stub_timer;
WindSensor stub_windsensor;
Tacker tacker(&stub_helm, &stub_compass, &stub_timer, &stub_windsensor);

class TackerTest : public ::testing::Test {
 protected:
  TackerTest() {}

  void SetUp() override {
  }

};

TEST_F(TackerTest, should_do_something) {
  EXPECT_EQ(1,1);
}

TEST_F(TackerTest, Slightly_strange_testing_of_absolute_wind_direction_using_mock) {
  stub_windsensor.set_relative(40);
  EXPECT_EQ(stub_windsensor.absolute(40), 80);
  EXPECT_EQ(stub_windsensor.absolute(10), 50);
  EXPECT_EQ(stub_windsensor.absolute(100), 140);
  EXPECT_EQ(stub_windsensor.absolute(245), 285);
  EXPECT_EQ(stub_windsensor.absolute(330), 10);

  stub_windsensor.set_relative(-170);
  EXPECT_EQ(stub_windsensor.absolute(40), 230);
  EXPECT_EQ(stub_windsensor.absolute(10), 200);
  EXPECT_EQ(stub_windsensor.absolute(100), 290);
  EXPECT_EQ(stub_windsensor.absolute(245), 75);
  EXPECT_EQ(stub_windsensor.absolute(330), 160);

}

// Does not work because we have state in Helm, which we don't properly reset between tests (sigh)
// TEST_F(TackerTest, Should_steer_directly_if_out_of_no_go_region) {
//   stub_windsensor.set_relative(90);
//   uangle bearing = 0;
//   stub_compass.set_bearings(&bearing,1);
//   helm.steer(20, 1, 1);
//   EXPECT_EQ(rudder_position(), -10);
// }


// (introduce the absolute wind direction)
// tack if the requested course is in the no-go zone
// choose the longer tack first

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
