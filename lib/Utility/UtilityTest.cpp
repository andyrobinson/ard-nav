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
} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
