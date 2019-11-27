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
  int servo_position(int pos) {
    return 90 - pos;
  }

};

TEST_F(SailTest, Stub_servo_should_record_last_position) {
  Servo servo;
  servo.write(88);
  EXPECT_EQ(servo.write_last_called(), 88);
}

TEST_F(SailTest, Should_Put_Sail_Straight_When_In_Irons) {
//  EXPECT_EQ(sail.sail_position(0),0);
  sail.set_position(0);
  EXPECT_EQ(stub_servo.write_last_called(), servo_position(0));
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Starboard) {
  EXPECT_EQ(sail.sail_position(NO_GO_LIMIT),0);
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Port) {
  EXPECT_EQ(sail.sail_position(-NO_GO_LIMIT),0);
}

TEST_F(SailTest, Should_Put_Sail_Angle_of_attack_at_20_on_Tack) {
  EXPECT_EQ(sail.sail_position(45),45 - ANGLE_OF_ATTACK);
  EXPECT_EQ(sail.sail_position(55),55 - ANGLE_OF_ATTACK);
  EXPECT_EQ(sail.sail_position(65),65 - ANGLE_OF_ATTACK);
  EXPECT_EQ(sail.sail_position(75),75 - ANGLE_OF_ATTACK);
  EXPECT_EQ(sail.sail_position(90),90 - ANGLE_OF_ATTACK);

  EXPECT_EQ(sail.sail_position(-65),-65 + ANGLE_OF_ATTACK);
}

TEST_F(SailTest, Should_Allow_Wind_to_increase_angle_of_attack_between_90_145) {
  EXPECT_EQ(sail.sail_position(100),70);
  EXPECT_EQ(sail.sail_position(110),70);
  EXPECT_EQ(sail.sail_position(120),70);
  EXPECT_EQ(sail.sail_position(130),70);
  EXPECT_EQ(sail.sail_position(140),70);

  EXPECT_EQ(sail.sail_position(-110),-70);

}

}  //namespace


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
