#include "Angle.h"
#include "gtest/gtest.h"

using namespace Angle;

namespace {

class AngleTest : public ::testing::Test {
};

  TEST_F(AngleTest, Should_add_one_signed_angle_to_another) {
    EXPECT_EQ(clockwise(10,30),40);
  }

  TEST_F(AngleTest, Should_wrap_around_when_we_pass_180) {
    EXPECT_EQ(clockwise(100,90),-170);
  }

  TEST_F(AngleTest, Should_wrap_around_when_we_make_exactly_180) {
    EXPECT_EQ(clockwise(142,38),-180);
  }

  TEST_F(AngleTest, Should_go_from_negative_to_positive) {
    EXPECT_EQ(clockwise(-40,50),10);
  }

  TEST_F(AngleTest, Should_go_from_positive_to_negative) {
    EXPECT_EQ(anticlock(70,120),-50);
  }

  TEST_F(AngleTest, Should_subtract_larger_values) {
    EXPECT_EQ(anticlock(-170,240),-50);
  }

  TEST_F(AngleTest, Should_wrap_around) {
    EXPECT_EQ(anticlock(-170,60),130);
  }

  TEST_F(AngleTest, AShould_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(uadd(40,40),80);
  }

  TEST_F(AngleTest, BShould_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(uadd(-160,200),40);
  }

  TEST_F(AngleTest, CShould_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(uadd(40,-180),-140);
  }

  TEST_F(AngleTest, DShould_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(uadd(-90,-40),-130);
  }

  // uangle -unsigned angle 0 - 359
  TEST_F(AngleTest, UShould_add_one_signed_angle_to_another) {
    EXPECT_EQ(uclockwise(10,30),40);
  }

  TEST_F(AngleTest, UShould_wrap_around_when_we_pass_359) {
    EXPECT_EQ(uclockwise(300,90),30);
  }

  TEST_F(AngleTest, UShould_wrap_around_when_we_make_exactly_360) {
    EXPECT_EQ(uclockwise(322,38),0);
  }

  TEST_F(AngleTest, UShould_subtract_one_signed_angle_to_another) {
    EXPECT_EQ(uanticlock(270,30),240);
  }

  TEST_F(AngleTest, UShould_wrap_around) {
    EXPECT_EQ(uanticlock(40,90),310);
  }

  TEST_F(AngleTest, UShould_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(uadd(40,40),80);
    EXPECT_EQ(uadd(200,200),40);
    EXPECT_EQ(uadd(40,-180),220);
    EXPECT_EQ(uadd(270,-40),230);
  }

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
