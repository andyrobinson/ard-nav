#include "Utility.h"
#include "gtest/gtest.h"

using namespace Utility;

namespace {

  class UtilityTest : public ::testing::Test {};

  void hexprintln(uint8_t *buff, int size) {
      char hex_string[3];
      for (int i=0; i < size; i++) {
          sprintf(hex_string, "%02X\0", buff[i]);
          std::cout << hex_string;
      }
      std::cout << "\n";
  }

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

  TEST_F(UtilityTest, Should_calculate_array_size) {
    int arr[] = {1, 2, 3, 4, 5};
    double darr[] = {345.87, 54.77, 33.2345};
    EXPECT_EQ(ARRAY_SIZE(arr),5);
    EXPECT_EQ(ARRAY_SIZE(darr),3);
  }

  TEST_F(UtilityTest, Should_stuff_a_long_into_a_byte_array) {
    long a = -1234L;
    uint8_t buff[4];
    stuff(a,buff,0,4);

    long b = buff[3] << 24 | buff[2] << 16 | buff[1] << 8 | buff[0];
    EXPECT_EQ(a,b);
  }

  TEST_F(UtilityTest, Should_stuff_into_the_middle_of_an_array) {
    long a = 333L;
    uint8_t buff[] = "ZZZZZZ";
    stuff(a,buff,1,4);

    long b = buff[4] << 24 | buff[3] << 16 | buff[2] << 8 | buff[1];
    EXPECT_EQ(a,b);
    EXPECT_EQ(buff[0],'Z');
    EXPECT_EQ(buff[5],'Z');
  }

  TEST_F(UtilityTest, Should_stuff_a_short_into_a_byte_array) {
    short a = -5768;
    uint8_t buff[2];
    stuff(a,buff,0,2);

    short b = buff[1] << 8 | buff[0];
    EXPECT_EQ(a,b);
  }


} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
