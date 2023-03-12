#include "Rudder.h"
#include "gtest/gtest.h"
#include "MServo.h"

namespace {

MServo stub_servo;
Rudder rudder(&stub_servo);

class RudderTest : public ::testing::Test {
 protected:
  RudderTest() {}
};

TEST_F(RudderTest, Stub_servo_should_record_last_position) {
  MServo servo;
  servo.write(2, 42);
  EXPECT_EQ(servo.angle_last_called, 42);
  EXPECT_EQ(servo.channel_last_called, 2);
}

TEST_F(RudderTest, Should_initialise_speeed_and_accel) {
  rudder.begin();
  EXPECT_EQ(stub_servo.channel_last_called, 0);
  EXPECT_EQ(stub_servo.speed_last_called, RUDDER_SPEED);
  EXPECT_EQ(stub_servo.accel_last_called, RUDDER_ACCEL);
}

TEST_F(RudderTest, Should_move_rudder_to_centre_at_90_degrees) {
  rudder.set_position(0);
  EXPECT_EQ(stub_servo.angle_last_called, 90);
}

TEST_F(RudderTest, Should_move_rudder_to_left) {
  rudder.set_position(40);
  EXPECT_EQ(stub_servo.angle_last_called, 90 + 38);
}

TEST_F(RudderTest, Should_move_rudder_to_right) {
  rudder.set_position(-20);
  EXPECT_EQ(stub_servo.angle_last_called, 90 - 20);
}

TEST_F(RudderTest, Should_not_move_rudder_to_right_beyond_limits) {
  rudder.set_position(-50);
  EXPECT_EQ(stub_servo.angle_last_called, 90 - RUDDER_MAX_DISPLACEMENT);
}

TEST_F(RudderTest, Should_not_move_rudder_to_left_beyond_limits) {
  rudder.set_position(60);
  EXPECT_EQ(stub_servo.angle_last_called, 90 + RUDDER_MAX_DISPLACEMENT);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
