#include "LiftDrag.h"
#include "gtest/gtest.h"

using namespace LiftDrag;

namespace {

class LiftDragTest : public ::testing::Test {
 protected:
  LiftDragTest() {}
};

TEST_F(LiftDragTest, drag_should_increase_up_until_90) {
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

TEST_F(LiftDragTest, drag_should_limit_angle_of_attack_to_0_180) {
    ASSERT_DOUBLE_EQ(Cd(0), Cd(-5));
    ASSERT_DOUBLE_EQ(Cd(180),Cd(200));
}

TEST_F(LiftDragTest, lift_should_increase_up_until_25) {
    ASSERT_DOUBLE_EQ(0.0,Cl(0));
    ASSERT_DOUBLE_EQ(0.0,Cl(5));
    ASSERT_DOUBLE_EQ(0.7,Cl(10));
    ASSERT_DOUBLE_EQ(1.2,Cl(15));
    ASSERT_DOUBLE_EQ(1.5,Cl(20));
    ASSERT_DOUBLE_EQ(1.6,Cl(25));
}

TEST_F(LiftDragTest, lift_should_decrease_untill_100_then_zero) {
    ASSERT_DOUBLE_EQ(1.6,Cl(25));
    ASSERT_DOUBLE_EQ(1.59264,Cl(30));
    ASSERT_DOUBLE_EQ(1.422,Cl(50));
    ASSERT_DOUBLE_EQ(1.02384,Cl(70));
    ASSERT_DOUBLE_EQ(0.39816000000000029,Cl(90));
    ASSERT_DOUBLE_EQ(0.0,Cl(100));
    ASSERT_DOUBLE_EQ(0.0,Cl(110));
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
