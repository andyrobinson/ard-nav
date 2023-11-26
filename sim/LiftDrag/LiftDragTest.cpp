#include "LiftDrag.h"
#include "gtest/gtest.h"

using namespace LiftDrag;

namespace {

class LiftDragTest : public ::testing::Test {
 protected:
  LiftDragTest() {}
};

TEST_F(LiftDragTest, cd_should_increase_up_until_90_then_decrease) {
    double cd = Cd(0);
    for (angle i = 10; i <= 90; i += 5) {
        ASSERT_GT(Cd(i),cd);
        ASSERT_LT(Cd(i), 1.6);
        cd = Cd(i);
    }
    ASSERT_LT(Cd(100),Cd(90));
    ASSERT_LT(Cd(110), Cd(100));

}

TEST_F(LiftDragTest, cd_should_limit_angle_of_attack_to_0_180) {
    ASSERT_DOUBLE_EQ(Cd(0), Cd(-5));
    ASSERT_DOUBLE_EQ(Cd(180),Cd(200));
}

TEST_F(LiftDragTest, cl_should_increase_up_until_25) {
    double cl = Cl(0);
    ASSERT_DOUBLE_EQ(0.0,cl);
    for (angle i = 10; i <= 25; i++) {
        ASSERT_GT(Cl(i),cl);
        ASSERT_LT(Cl(i), 1.61);
        cl = Cl(i);
    }
}

TEST_F(LiftDragTest, cl_should_decrease_from_25_until_90) {
    double cl = Cl(25);
    ASSERT_DOUBLE_EQ(1.6,cl);
    for (angle i = 26; i <= 27; i++) {
        ASSERT_LT(Cl(i),cl);
        cl = Cl(i);
    }
}

TEST_F(LiftDragTest, cl_should_be_zero_from_90_onwards) {
    for (angle i = 90; i <= 180; i++) {
        ASSERT_DOUBLE_EQ(0.0, Cl(i));
    }
}

TEST_F(LiftDragTest, lift_should_increase_to_25) {
    double lf = lift(0,10.0);
    ASSERT_DOUBLE_EQ(0.0,lf);

    for (angle i = 1; i<=25; i++) {
        ASSERT_GE(lift(i,10.0),lf);
        lf = lift(i,10.0);
    }
}

TEST_F(LiftDragTest, lift_should_decrease_from_25) {
    double lf = lift(25,10.0);

    for (angle i = 26; i<=90; i++) {
        ASSERT_LE(lift(i,10.0),lf);
        lf = lift(i,10.0);
    }
}


TEST_F(LiftDragTest, lift_should_always_be_positive) {
    ASSERT_GE(lift(-25,10.0),0.0);
    ASSERT_GE(lift(-50,10.0),0.0);
    ASSERT_GE(lift(-80,10.0),0.0);
}

TEST_F(LiftDragTest, lift_should_increase_with_square_of_wind_speed) {
    ASSERT_DOUBLE_EQ(3.612,lift(25,5.0));
    ASSERT_DOUBLE_EQ(14.448,lift(25,10.0)); // 4 times
    ASSERT_DOUBLE_EQ(57.792,lift(25,20.0)); // 4 times
}

TEST_F(LiftDragTest, drag_should_increase_to_90) {
    ASSERT_DOUBLE_EQ(1.806,drag(0,10.0));
    ASSERT_DOUBLE_EQ(1.9331830985915497,drag(10,10.0));
    ASSERT_DOUBLE_EQ(2.3147323943661973,drag(20,10.0));
    ASSERT_DOUBLE_EQ(2.9506478873239437,drag(30,10.0));
    ASSERT_DOUBLE_EQ(3.8409295774647889,drag(40,10.0));
    ASSERT_DOUBLE_EQ(4.985577464788733,drag(50,10.0));
    ASSERT_DOUBLE_EQ(6.3845915492957745,drag(60,10.0));
    ASSERT_DOUBLE_EQ(8.0379718309859154,drag(70,10.0));
    ASSERT_DOUBLE_EQ(9.9457183098591564,drag(80,10.0));
    ASSERT_DOUBLE_EQ(12.107830985915493,drag(90,10.0));
}
}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
