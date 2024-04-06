#define DELAY_BETWEEN_READINGS 1001

#include "Battery.h"
#include "gtest/gtest.h"

Timer stub_timer;

// provides a way of giving an array of values to the pin, but it would be easy to buffer overrun
class StubPin {
  public:
    StubPin(){};
    StubPin(int *lipoValues1, int *lipoValues2) {
        lv1=lipoValues1;
        lv2=lipoValues2;
        i1=-1;
        i2=-1;
    }

    int pinFn(uint8_t pin) {
      if (pin == LIPO1) {
        i1++;
        return lv1[i1];
      } else if (pin == LIPO2) {
        i2++;
        return lv2[i2];
      } else return -1;
    }
    
  private: 
    int *lv1,*lv2;
    int i1,i2;
  
};

StubPin stubby;

int pinFn(uint8_t pin) {
  return stubby.pinFn(pin);
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
    int maxlipo1 = 512; // 0.5
    int maxlipo2 = 256; // 0.25
    int readings1[5] = {100,maxlipo1,350,400,300};
    int readings2[5] = {200,220,230,maxlipo2,225};
    stubby = StubPin(readings1,readings2);

    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<4;j++) {battery.lipomaxv(0); stub_timer.wait(DELAY_BETWEEN_READINGS);}

    EXPECT_NEAR(battery.lipomaxv(0),3.3,0.001);
    EXPECT_NEAR(battery.lipomaxv(1),1.65,0.001);
  }

  TEST_F(BatteryTest, anything_above_1_is_treated_as_1) {
    int maxlipo1 = 512; // 0.5
    int maxlipo2 = 256; // 0.25
    int readings1[5] = {100,maxlipo1,350,400,300};
    int readings2[5] = {200,220,230,maxlipo2,225};
    stubby = StubPin(readings1,readings2);

    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<4;j++) {battery.lipomaxv(0); stub_timer.wait(DELAY_BETWEEN_READINGS);}

    EXPECT_NEAR(battery.lipomaxv(0),3.3,0.001);
    EXPECT_NEAR(battery.lipomaxv(1),1.65,0.001);
    EXPECT_NEAR(battery.lipomaxv(2),1.65,0.001);
    EXPECT_NEAR(battery.lipomaxv(200),1.65,0.001);
  }


  TEST_F(BatteryTest, should_return_min_reading_in_volts) {
    int minlipo1 = 85;
    int minlipo2 = 125;
    int readings1[5] = {minlipo1,512, 90, 400, 300};
    int readings2[5] = {150, minlipo2, 150, 150, 150};
    stubby = StubPin(readings1,readings2);

    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<3;j++) {battery.lipominv(0); stub_timer.wait(DELAY_BETWEEN_READINGS);}

    EXPECT_NEAR(battery.lipominv(0),(6.6 * minlipo1)/1024.0,0.001);
    EXPECT_NEAR(battery.lipominv(1),(6.6 * minlipo2)/1024.0,0.001);
  }

  TEST_F(BatteryTest, new_maximum_should_replace_old_when_window_moves_on_even_if_smaller) {
    int readings1[25] = {100,512,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,481};
    int readings2[25] = {256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256};
    stubby = StubPin(readings1,readings2);
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<19;j++) {battery.lipomaxv(0);stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading1a = battery.lipomaxv(0);stub_timer.wait(DELAY_BETWEEN_READINGS);
    for (int j=0; j<4;j++) {battery.lipomaxv(0);stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading1b = battery.lipomaxv(0);
    float reading2 = battery.lipomaxv(1);

    EXPECT_NEAR(reading1a,3.3,0.001);
    EXPECT_NEAR(reading1b,3.1,0.001);
    EXPECT_NEAR(reading2,1.65,0.001);
  }

  TEST_F(BatteryTest, new_minimum_should_replace_old_one_even_if_bigger) {
    int readings1[25] = {256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256};
    int readings2[25] = {100,512,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,400,300,300,150,200,200,200};
    stubby = StubPin(readings1,readings2);
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<19;j++) {battery.lipomaxv(1);stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading2a = battery.lipominv(1);stub_timer.wait(DELAY_BETWEEN_READINGS);
    for (int j=0; j<4;j++) {battery.lipominv(1);stub_timer.wait(DELAY_BETWEEN_READINGS);}
    float reading2b = battery.lipominv(1);
    float reading1 = battery.lipominv(0);

    EXPECT_NEAR(reading1,1.65,0.001);
    EXPECT_NEAR(reading2a,0.644,0.001);
    EXPECT_NEAR(reading2b,0.966,0.001);
  }

  TEST_F(BatteryTest, should_return_raw_values) {
    int readings1[10] = {100, 512,300,400,300,300,400,300,300,99};
    int readings2[10] = {50, 512,300,400,300,300,400,300,300,750};
    stubby = StubPin(readings1,readings2);
    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<10;j++) {battery.lipomaxv(0);stub_timer.wait(DELAY_BETWEEN_READINGS);}
    uint16_t min_reading1 = battery.raw_min(0);
    uint16_t max_reading1 = battery.raw_max(0);
    uint16_t min_reading2 = battery.raw_min(1);
    uint16_t max_reading2 = battery.raw_max(1);

    EXPECT_EQ(min_reading1, 99);
    EXPECT_EQ(max_reading1,512);
    EXPECT_EQ(min_reading2, 50);
    EXPECT_EQ(max_reading2,750);
  }

  TEST_F(BatteryTest, should_space_out_readings_by_given_delay) {

    int readings[10] = {102, 102, 512, 90, 400, 300, 10, 10, 10, 10};
    stubby = StubPin(readings,readings);

    Battery battery(&pinFn, &stub_timer);

    for (int j=0; j<3;j++) { 
      battery.lipominv(0);
      battery.lipomaxv(0);
      battery.lipominv(1);
      battery.lipomaxv(1);
    }
    float max1 = battery.lipomaxv(0), min1 = battery.lipominv(0);

    for (int j=0; j<2;j++) { 
      battery.lipominv(0);
      battery.lipomaxv(0);
      battery.lipominv(1);
      battery.lipomaxv(1);
      stub_timer.wait(DELAY_BETWEEN_READINGS);
    }
    float max2 = battery.lipomaxv(0), min2 = battery.lipominv(0);

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
