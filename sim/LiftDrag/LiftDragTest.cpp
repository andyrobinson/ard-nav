#include "LiftDrag.h"
#include "gtest/gtest.h"

using namespace LiftDrag;

namespace {

class LiftDragTest : public ::testing::Test {
 protected:
  LiftDragTest() {}
};

TEST_F(LiftDragTest, cd_should_increase_up_until_90) {
    ASSERT_DOUBLE_EQ(0.2,Cd(0));
    ASSERT_DOUBLE_EQ(0.21408450704225354,Cd(10));
    ASSERT_DOUBLE_EQ(0.25633802816901408,Cd(20));
    ASSERT_DOUBLE_EQ(0.3267605633802817,Cd(30));
    ASSERT_DOUBLE_EQ(0.42535211267605633,Cd(40));
    ASSERT_DOUBLE_EQ(0.55211267605633796,Cd(50));
    ASSERT_DOUBLE_EQ(0.70704225352112671,Cd(60));
    ASSERT_DOUBLE_EQ(0.89014084507042246,Cd(70));
    ASSERT_DOUBLE_EQ(1.1014084507042254,Cd(80));
    ASSERT_DOUBLE_EQ(1.3408450704225352,Cd(90));
    ASSERT_DOUBLE_EQ(1.1014084507042254,Cd(100));
    ASSERT_DOUBLE_EQ(0.89014084507042246,Cd(110));
}

TEST_F(LiftDragTest, cd_should_limit_angle_of_attack_to_0_180) {
    ASSERT_DOUBLE_EQ(Cd(0), Cd(-5));
    ASSERT_DOUBLE_EQ(Cd(180),Cd(200));
}

TEST_F(LiftDragTest, cl_should_increase_up_until_25) {
    ASSERT_DOUBLE_EQ(0.0,Cl(0));
    ASSERT_DOUBLE_EQ(0.0,Cl(5));
    ASSERT_DOUBLE_EQ(0.7,Cl(10));
    ASSERT_DOUBLE_EQ(1.2,Cl(15));
    ASSERT_DOUBLE_EQ(1.5,Cl(20));
    ASSERT_DOUBLE_EQ(1.6,Cl(25));
}

TEST_F(LiftDragTest, cl_should_decrease_from_25_until_100_then_zero) {
    ASSERT_DOUBLE_EQ(1.6,Cl(25));
    ASSERT_DOUBLE_EQ(1.59264,Cl(30));
    ASSERT_DOUBLE_EQ(1.422,Cl(50));
    ASSERT_DOUBLE_EQ(1.02384,Cl(70));
    ASSERT_DOUBLE_EQ(0.39816000000000029,Cl(90));
    ASSERT_DOUBLE_EQ(0.0,Cl(100));
    ASSERT_DOUBLE_EQ(0.0,Cl(110));
}

TEST_F(LiftDragTest, lift_should_increase_to_25_then_tail_off) {
    ASSERT_DOUBLE_EQ(0.0,lift(0,10.0));
    ASSERT_DOUBLE_EQ(6.321,lift(10,10.0));
    ASSERT_DOUBLE_EQ(13.545,lift(20,10.0));
    ASSERT_DOUBLE_EQ(14.448,lift(25,10.0));
    ASSERT_DOUBLE_EQ(14.3815392,lift(30,10.0));
    ASSERT_DOUBLE_EQ(13.8679128,lift(40,10.0));
    ASSERT_DOUBLE_EQ(12.84066,lift(50,10.0));
    ASSERT_DOUBLE_EQ(11.2997808,lift(60,10.0));
    ASSERT_DOUBLE_EQ(9.2452752,lift(70,10.0));
    ASSERT_DOUBLE_EQ(6.6771432,lift(80,10.0));
    ASSERT_DOUBLE_EQ(3.5953848000000033,lift(90,10.0));
    ASSERT_DOUBLE_EQ(0.0,lift(100,10.0));
}

TEST_F(LiftDragTest, lift_should_always_be_positive) {
    ASSERT_DOUBLE_EQ(14.448,lift(-25,10.0));
    ASSERT_DOUBLE_EQ(12.84066,lift(-50,10.0));
    ASSERT_DOUBLE_EQ(6.6771432,lift(-80,10.0));
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
