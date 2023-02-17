#define A0 99 // stub port number
#define DELAY_BETWEEN_READINGS 1001

#include "Battery.h"
#include "gtest/gtest.h"

// provides a way of giving an array of values to the pin, but it would be easy to buffer overrun
int pin_index = -1;
int *values;
Timer stub_timer;

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
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<3;j++) {battery.lipo1maxv(); stub_timer.wait(DELAY_BETWEEN_READINGS);}

    EXPECT_NEAR(battery.lipo1maxv(),3.3,0.001);
  }

  TEST_F(BatteryTest, should_return_min_reading_in_volts) {
    pin_index = -1;
    int myvalues[5] = {85, 512, 90,400,300};
    values = myvalues;
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<3;j++) {battery.lipo1minv(); stub_timer.wait(DELAY_BETWEEN_READINGS);}

    EXPECT_NEAR(battery.lipo1minv(),(6.6 * 85.0)/1024.0,0.001);
  }

  TEST_F(BatteryTest, new_maximum_should_replace_old_one_even_if_smaller) {
    pin_index = -1;
    int myvalues[25] = {100, 512,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,481};
    values = myvalues;
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<19;j++) {battery.lipo1maxv();stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading1 = battery.lipo1maxv();stub_timer.wait(DELAY_BETWEEN_READINGS);
    for (int j=0; j<4;j++) {battery.lipo1maxv();stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading2 = battery.lipo1maxv();

    EXPECT_NEAR(reading1,3.3,0.001);
    EXPECT_NEAR(reading2,3.1,0.001);
  }

  TEST_F(BatteryTest, new_minimum_should_replace_old_one_even_if_bigger) {
    pin_index = -1;
    int myvalues[25] = {100, 512,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,150,200,200,200};
    values = myvalues;
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<19;j++) {battery.lipo1maxv();stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading1 = battery.lipo1minv();stub_timer.wait(DELAY_BETWEEN_READINGS);
    for (int j=0; j<4;j++) {battery.lipo1minv();stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading2 = battery.lipo1minv();

    EXPECT_NEAR(reading1,0.644,0.001);
    EXPECT_NEAR(reading2,0.966,0.001);
  }

  TEST_F(BatteryTest, should_space_out_readings_by_given_delay) {
    pin_index = -1;
    int myvalues[10] = {102, 102, 512, 90, 400, 300, 10, 10, 10, 10};
    values = myvalues;

    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<3;j++) { 
      battery.lipo1minv();
      battery.lipo1maxv();
    }
    float max1 = battery.lipo1maxv(), min1 = battery.lipo1minv();

    for (int j=0; j<2;j++) { 
      battery.lipo1minv();
      battery.lipo1maxv();
      stub_timer.wait(DELAY_BETWEEN_READINGS);
    }
    float max2 = battery.lipo1maxv(), min2 = battery.lipo1minv();

    EXPECT_NEAR(max1,0.657,0.001);
    EXPECT_NEAR(min1,0.657,0.001);
    EXPECT_NEAR(min2,0.657,0.001);
    EXPECT_NEAR(max2,3.3,0.001);
  }

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
