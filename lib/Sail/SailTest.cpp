#include "Sail.h"
#include "gtest/gtest.h"
#include "VSServoSamd.h"

#include <iostream>

namespace {

VSServoSamd stub_servo;
Sail sail(&stub_servo);
uangle NO_GO_LIMIT = ANGLE_OF_ATTACK;

class SailTest : public ::testing::Test {
 protected:
  SailTest() {
    //sail = Sail(stub_servo);
     // You can do set-up work for each test here.
  }

  ~SailTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Objects declared here can be used by all tests in the test suite for Foo.
  int servo_position(int pos_90_each_side_of_zero) {
    int servo_0_to_180 = 90 - pos_90_each_side_of_zero;
    return servo_0_to_180;
  }

};

TEST_F(SailTest, Stub_servo_should_record_last_position) {
  VSServoSamd servo;
  servo.write(88);
  EXPECT_EQ(servo.write_last_called(), 88);
}

TEST_F(SailTest, Should_Put_Sail_Straight_When_In_Irons) {
  sail.set_position(0);
  EXPECT_EQ(stub_servo.write_last_called(), servo_position(0));
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Starboard) {
  sail.set_position(NO_GO_LIMIT);
  EXPECT_EQ(stub_servo.write_last_called(), servo_position(0));
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Port) {
  sail.set_position(-NO_GO_LIMIT);
  EXPECT_EQ(stub_servo.write_last_called(), servo_position(0));
}

TEST_F(SailTest, Should_maintain_angle_of_attack_when_close_hauled) {

  for (short i = NO_GO_LIMIT; i <= PURE_LIFT_LIMIT; i++) {
    sail.set_position(i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(i - ANGLE_OF_ATTACK));

    sail.set_position(-i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(-i + ANGLE_OF_ATTACK));
  }

}

TEST_F(SailTest, Should_fix_sail_allowing_Wind_to_increase_angle_of_attack_moving_from_lift_to_drag) {

  short fixed_sail = PURE_LIFT_LIMIT - ANGLE_OF_ATTACK;
  for (short i = PURE_LIFT_LIMIT; i <= LIFT_TO_DRAG_LIMIT; i++) {
    sail.set_position(i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(fixed_sail));

    sail.set_position(-i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(-fixed_sail));
  }
}

TEST_F(SailTest, Should_increase_drag_after_lift_to_drag_limit) {
  short fixed_sail = PURE_LIFT_LIMIT - ANGLE_OF_ATTACK;
  for (short i = LIFT_TO_DRAG_LIMIT; i <= 180 - GYBE_CHECK_MAX_DIFF; i++) {
    short expected_position = ((i - LIFT_TO_DRAG_LIMIT) * (90 - fixed_sail))/(180 - LIFT_TO_DRAG_LIMIT) + fixed_sail;
    sail.set_position(i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(expected_position));

    sail.set_position(-i);
    EXPECT_EQ(stub_servo.write_last_called(),servo_position(-expected_position));
  }

}

TEST_F(SailTest, Should_not_increase_sail_angle_beyond_90) {
  sail.set_position(-200);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-90));

  sail.set_position(0); // to foil anti-gybe
  sail.set_position(190);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_but_set_to_max_for_10_degrees_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(82));
  sail.set_position(-178);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_if_still_on_same_side) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(-170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-82));
  sail.set_position(-179);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-89));
}

TEST_F(SailTest, Should_gybe_sail_if_more_than_10_degress_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(168);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(81));
  sail.set_position(-168);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-81));
}

TEST_F(SailTest, Should_set_sail_to_30_degrees_if_windsensor_not_working) {
  sail.set_position(-999); // see Windsensor.h
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(30));
}

// TEST_F(SailTest, Print_out_wind_and_sail_values_not_arduino) {
//   for (int i=-180; i<=180; i++) {
//     sail.set_position(i);
//     int s = stub_servo.write_last_called(); // see Windsensor.h
//     std::cout << i;
//     std::cout << ' ';
//     std::cout << s;
//     std::cout << '\n';
//   }
// }
}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
