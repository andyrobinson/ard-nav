#include "Utility.h"
#include "gtest/gtest.h"

using namespace Utility;

namespace {

  class UtilityTest : public ::testing::Test {};

  TEST_F(UtilityTest, Should_calculate_the_sign) {
    int a = -77;
    long b = 887685;
    short c = -88;
    float d = 7685.88;
    double e = -984759867.99;
    int zero = 0;
    EXPECT_EQ(sign(a),-1);
    EXPECT_EQ(sign(b),1);
    EXPECT_EQ(sign(c),-1);
    EXPECT_EQ(sign(d),1.0);
    EXPECT_EQ(sign(e),-1.0);
    EXPECT_EQ(sign(zero),0);
  }

  TEST_F(UtilityTest, Should_calculate_the_absolute_value) {
    int a = -33;
    long b = 88;
    short c = -66;
    float d = 85.88;
    double e = -98.99;
    int zero = 0;
    EXPECT_EQ(abs1(a),33);
    EXPECT_EQ(abs1(b),88);
    EXPECT_EQ(abs1(c),66);
    EXPECT_EQ(abs1(d),(float) 85.88);
    EXPECT_EQ(abs1(e),98.99);
    EXPECT_EQ(abs1(zero),0);
  }

  TEST_F(UtilityTest, Should_calculate_the_max_and_min) {
    int a1 = -33;
    int a2 = -46;
    long b1 = 88;
    long b2 = 89;
    short c1 = -66;
    short c2 = -42;
    float d1 = 85.88;
    float d2 = -0.23;
    double e1 = -98.99;
    double e2 = 12345.99;

    EXPECT_EQ(min1(a1,a2),a2);
    EXPECT_EQ(min1(b1,b2),b1);
    EXPECT_EQ(min1(c1,c2),c1);
    EXPECT_EQ(min1(d1,d2),d2);
    EXPECT_EQ(min1(e1,e2),e1);

    EXPECT_EQ(max1(a1,a2),a1);
    EXPECT_EQ(max1(b1,b2),b2);
    EXPECT_EQ(max1(c1,c2),c2);
    EXPECT_EQ(max1(d1,d2),d1);
    EXPECT_EQ(max1(e1,e2),e2);
  }


  TEST_F(UtilityTest, Should_calculate_the_unix_time) {
    EXPECT_EQ(unix_time(0,1,1,0,0,0),946684800); // 1st Jan 2000, first date available
    EXPECT_EQ(unix_time(0,1,2,10,10,0),946807800);
    EXPECT_EQ(unix_time(0,02,15,18,46,0),950640360);
    EXPECT_EQ(unix_time(0,6,30,8,46,0),962354760);
    EXPECT_EQ(unix_time(10,11,1,23,59,59),1288655999);
    EXPECT_EQ(unix_time(20,2,14,22,19,33),1581718773);
    EXPECT_EQ(unix_time(20,2,29,23,59,59),1583020799);
    EXPECT_EQ(unix_time(20,3,1,00,00,00),1583020800);
    EXPECT_EQ(unix_time(20,1,1,00,00,00),1577836800);
  }

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
