#include "Sail.h"
#include "gtest/gtest.h"
#include "Servo.h"

namespace {

Servo stub_servo;
Sail sail(&stub_servo);

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
  Servo servo;
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

TEST_F(SailTest, Should_Put_Sail_Angle_of_attack_at_20_on_Tack) {
  sail.set_position(45);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(45 - ANGLE_OF_ATTACK));
  sail.set_position(55);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(55 - ANGLE_OF_ATTACK));
  sail.set_position(65);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(65 - ANGLE_OF_ATTACK));
  sail.set_position(75);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(75 - ANGLE_OF_ATTACK));
  sail.set_position(90);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(90 - ANGLE_OF_ATTACK));

  sail.set_position(-65);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-65 + ANGLE_OF_ATTACK));
}

TEST_F(SailTest, Should_Allow_Wind_to_increase_angle_of_attack_between_90_145) {
  sail.set_position(100);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(70));

  sail.set_position(110);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(70));
  sail.set_position(120);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(70));
  sail.set_position(130);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(70));
  sail.set_position(140);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(70));

  sail.set_position(-110);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-70));
}

TEST_F(SailTest, Should_increase_drag_after_lift_to_drag_limit) {
  sail.set_position(170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(85));

  sail.set_position(-150);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-75));

  sail.set_position(-160);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-80));

  sail.set_position(-170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-85));

  sail.set_position(-180);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-90));
}

TEST_F(SailTest, Should_not_increase_sail_angle_beyond_90) {
  sail.set_position(-200);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-90));

  sail.set_position(0); // to foil anti-gybe
  sail.set_position(190);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_but_set_to_max_for_10_degress_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(85));
  sail.set_position(-170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_if_still_on_same_side) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(-170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-85));
  sail.set_position(-179);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-89));
}

TEST_F(SailTest, Should_gybe_sail_if_more_than_10_degress_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(168);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(84));
  sail.set_position(-170);
  EXPECT_EQ(stub_servo.write_last_called(),servo_position(-85));
}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
