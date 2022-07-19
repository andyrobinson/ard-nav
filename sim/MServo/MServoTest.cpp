#include "MServo.h"
#include "gtest/gtest.h"

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
Boat boat(&chorlton);
MServo mservo(&boat);

class MServoTest : public ::testing::Test {
 protected:
  MServoTest() {}
};

TEST_F(MServoTest, should_set_rudder_on_boat_on_channel_0) {
  mservo.write(0,23);
  EXPECT_EQ(boat.rudder, 23);
}

// TEST_F(MServoTest, should_set_sail_on_boat_on_channel_1) {
//   mservo.write(1,=40);
//   EXPECT_EQ(boat.sail, -41);
// }


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
