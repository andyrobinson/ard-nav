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

  TEST_F(AngleTest, Should_work_with_both_signs_and_wrap_around) {
    EXPECT_EQ(add(40,40),80);
    EXPECT_EQ(add(-160,200),40);
    EXPECT_EQ(add(40,-180),-140);
    EXPECT_EQ(add(-90,-40),-130);
  }

  // conversion
  TEST_F(AngleTest, Should_convert_to_unsigned_angle) {
    EXPECT_EQ(to_uangle(0),0);
    EXPECT_EQ(to_uangle(-40),320);
    EXPECT_EQ(to_uangle(-170),190);
    EXPECT_EQ(to_uangle(-180),180);
    EXPECT_EQ(to_uangle(180),180);
    EXPECT_EQ(to_uangle(100),100);
    EXPECT_EQ(to_uangle(30),30);
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

  // conversion
  TEST_F(AngleTest, Should_convert_to_signed_angle) {
    EXPECT_EQ(to_angle(0),0);
    EXPECT_EQ(to_angle(40),40);
    EXPECT_EQ(to_angle(170),170);
    EXPECT_EQ(to_angle(179),179);
    EXPECT_EQ(to_angle(180),180);
    EXPECT_EQ(to_angle(181),-179);
    EXPECT_EQ(to_angle(260),-100);
    EXPECT_EQ(to_angle(330),-30);
  }

  TEST_F(AngleTest, UShould_find_udiff_expressed_as_signed_angle_from_a_to_b) {
    EXPECT_EQ(udiff(0,0),0);
    EXPECT_EQ(udiff(10,20),10);
    EXPECT_EQ(udiff(50,23),-27);
    EXPECT_EQ(udiff(333,211),-122);
  }

  TEST_F(AngleTest, UShould_never_exceed_180_in_either_direction) {
    EXPECT_EQ(udiff(200,10),170);
    EXPECT_EQ(udiff(15,225),-150);
  }

  TEST_F(AngleTest, UShould_correct_find_differences_across_zero) {
    EXPECT_EQ(udiff(350,5),15);
    EXPECT_EQ(udiff(10,345),-25);
  }
} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
