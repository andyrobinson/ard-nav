#include "Rudder.h"
#include "gtest/gtest.h"
#include "VSServoSamd.h"

namespace {

VSServoSamd stub_servo;
Rudder rudder(&stub_servo);

class RudderTest : public ::testing::Test {
 protected:
  RudderTest() {}
};

TEST_F(RudderTest, Stub_servo_should_record_last_position) {
  VSServoSamd servo;
  servo.write(42);
  EXPECT_EQ(servo.write_last_called(), 42);
}

TEST_F(RudderTest, Should_move_rudder_to_centre_at_90_degrees) {
  rudder.set_position(0);
  EXPECT_EQ(stub_servo.write_last_called(), 90);
}

TEST_F(RudderTest, Should_move_rudder_to_left) {
  rudder.set_position(40);
  EXPECT_EQ(stub_servo.write_last_called(), 90 - 40);
}

TEST_F(RudderTest, Should_move_rudder_to_right) {
  rudder.set_position(-20);
  EXPECT_EQ(stub_servo.write_last_called(), 90 + 20);
}

TEST_F(RudderTest, Should_not_move_rudder_to_right_beyond_limits) {
  rudder.set_position(-50);
  EXPECT_EQ(stub_servo.write_last_called(), 90 + RUDDER_MAX_DISPLACEMENT);
}

TEST_F(RudderTest, Should_not_move_rudder_to_left_beyond_limits) {
  rudder.set_position(60);
  EXPECT_EQ(stub_servo.write_last_called(), 90 - RUDDER_MAX_DISPLACEMENT);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
