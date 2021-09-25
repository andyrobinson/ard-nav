#include "Sail.h"
#include "gtest/gtest.h"
#include "StubLogger.h"

#include <iostream>

namespace {

position chorlton = {53.44096, -2.27235, 5.0};
StubLogger logger;
Boat boat(&chorlton, &logger);
Sail sail(&boat);

uangle NO_GO_LIMIT = ANGLE_OF_ATTACK;

class SailTest : public ::testing::Test {
 protected:
  SailTest() {
  }

  int servo_position(int pos_90_each_side_of_zero) {
    int servo_0_to_180 = 90 - pos_90_each_side_of_zero;
    return servo_0_to_180;
  }

};

TEST_F(SailTest, Should_Put_Sail_Straight_When_In_Irons) {
  sail.set_position(0);
  EXPECT_EQ(boat.sail, servo_position(0));
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Starboard) {
  sail.set_position(NO_GO_LIMIT);
  EXPECT_EQ(boat.sail, servo_position(0));
}

TEST_F(SailTest, Should_Put_Sail_Straight_Up_to_No_go_limit_Port) {
  sail.set_position(-NO_GO_LIMIT);
  EXPECT_EQ(boat.sail, servo_position(0));
}

TEST_F(SailTest, Should_maintain_angle_of_attack_when_close_hauled) {

  for (short i = NO_GO_LIMIT; i <= PURE_LIFT_LIMIT; i++) {
    sail.set_position(i);
    EXPECT_EQ(boat.sail,servo_position(i - ANGLE_OF_ATTACK));

    sail.set_position(-i);
    EXPECT_EQ(boat.sail,servo_position(-i + ANGLE_OF_ATTACK));
  }

}

TEST_F(SailTest, Should_fix_sail_allowing_Wind_to_increase_angle_of_attack_moving_from_lift_to_drag) {

  short fixed_sail = PURE_LIFT_LIMIT - ANGLE_OF_ATTACK;
  for (short i = PURE_LIFT_LIMIT; i <= LIFT_TO_DRAG_LIMIT; i++) {
    sail.set_position(i);
    EXPECT_EQ(boat.sail,servo_position(fixed_sail));

    sail.set_position(-i);
    EXPECT_EQ(boat.sail,servo_position(-fixed_sail));
  }
}

TEST_F(SailTest, Should_increase_drag_after_lift_to_drag_limit) {
  short fixed_sail = PURE_LIFT_LIMIT - ANGLE_OF_ATTACK;
  for (short i = LIFT_TO_DRAG_LIMIT; i <= 180 - GYBE_CHECK_MAX_DIFF; i++) {
    short expected_position = ((i - LIFT_TO_DRAG_LIMIT) * (90 - fixed_sail))/(180 - LIFT_TO_DRAG_LIMIT) + fixed_sail;
    sail.set_position(i);
    EXPECT_EQ(boat.sail,servo_position(expected_position));

    sail.set_position(-i);
    EXPECT_EQ(boat.sail,servo_position(-expected_position));
  }

}

TEST_F(SailTest, Should_not_increase_sail_angle_beyond_90) {
  sail.set_position(-200);
  EXPECT_EQ(boat.sail,servo_position(-90));

  sail.set_position(0); // to foil anti-gybe
  sail.set_position(190);
  EXPECT_EQ(boat.sail,servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_but_set_to_max_for_10_degrees_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(170);
  EXPECT_EQ(boat.sail,servo_position(82));
  sail.set_position(-178);
  EXPECT_EQ(boat.sail,servo_position(90));
}

TEST_F(SailTest, Should_not_gybe_if_still_on_same_side) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(-170);
  EXPECT_EQ(boat.sail,servo_position(-82));
  sail.set_position(-179);
  EXPECT_EQ(boat.sail,servo_position(-89));
}

TEST_F(SailTest, Should_gybe_sail_if_more_than_10_degress_when_running) {
  sail.set_position(0); // to foil anti-gybe
  sail.set_position(168);
  EXPECT_EQ(boat.sail,servo_position(81));
  sail.set_position(-168);
  EXPECT_EQ(boat.sail,servo_position(-81));
}

TEST_F(SailTest, Should_set_sail_to_30_degrees_if_windsensor_not_working) {
  sail.set_position(-999); // see Windsensor.h
  EXPECT_EQ(boat.sail,servo_position(30));
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
