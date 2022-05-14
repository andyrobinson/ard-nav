#define A0 99 // stub port number

#include "Battery.h"
#include "gtest/gtest.h"

// provides a way of giving an array of values to the pin, but it would be easy to buffer overrun
int pin_index = -1;
int *values;

int pinFn(uint8_t pin) {
  pin_index++;
  return values[pin_index];
}

namespace {

  class BatteryTest : public ::testing::Test {
   protected:
    BatteryTest() {
       // You can do set-up work for each test here.
    }
  };

  // pin values
  // 1024 = 6.6v
  // 512 = 3.3v

  TEST_F(BatteryTest, should_return_max_reading_in_volts) {
    pin_index = -1;
    int myvalues[5] = {100, 512,300,400,300};
    values = myvalues;
    Battery battery(&pinFn);

    for (int j=0; j<3;j++) battery.lipo1maxv();

    EXPECT_NEAR(battery.lipo1maxv(),3.3,0.001);
  }

  TEST_F(BatteryTest, new_maximum_should_replace_old_one_even_if_smaller) {
    pin_index = -1;
    int myvalues[25] = {100, 512,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,481};
    values = myvalues;
    Battery battery(&pinFn);

    for (int j=0; j<19;j++) battery.lipo1maxv();
    float reading1 = battery.lipo1maxv();
    for (int j=0; j<4;j++) battery.lipo1maxv();
    float reading2 = battery.lipo1maxv();

    EXPECT_NEAR(reading1,3.3,0.001);
    EXPECT_NEAR(reading2,3.1,0.001);
  }


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
