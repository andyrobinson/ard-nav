#include "Compass.h"
#include "I2C.h"
#include "Timer.h"
#include "Angle.h"
#include "gtest/gtest.h"

namespace {

Timer stub_timer;
I2C stub_i2c;
Compass compass;

class CompassTest : public ::testing::Test {
 protected:
  CompassTest() {}

  void SetUp() override {
    compass = Compass(&stub_i2c, &stub_timer);
    stub_timer.set_millis(100);
    compass.begin();
  }
};

TEST_F(CompassTest, should_produce_minus_250) {
  int16_t expected = -250;
  uint8_t high = expected >= 0 ? 0 : 255;
  uint8_t low = (uint8_t) expected >=0 ? expected : 256 + expected;
  int16_t result = (int16_t)((uint16_t) low | ((uint16_t) high << 8));
  EXPECT_EQ(result, expected);
}

TEST_F(CompassTest, should_use_corrections_when_calculating_bearing) {
  // all 50s produces angle of 45 degrees
  uint8_t xlow = 50 - COMPASS_X_CORRECTION;
  uint8_t ylow = 50 - COMPASS_Y_CORRECTION;
  uint8_t zlow = 6; // 50 + 256 + COMPASS_Z_CORRECTION;
  uint8_t zhigh = 255; // 2s comp negative
  uint8_t readings[] = {xlow,0,ylow,0,zlow,zhigh,0,0,0,0,0,50};
  stub_i2c.set_results(readings,12);
  uangle bearing = compass.bearing();

  EXPECT_EQ(bearing, 45);
}

TEST_F(CompassTest, should_cache_the_value_for_10_ms) {
  uint8_t readings[] = {0,50,0,50,0,50,0,0,0,0,0,50,10,10,110,110,0,0,0,0,0,0,0,0,50};
  stub_i2c.set_results(readings,24);
  uangle bearing = compass.bearing();
  uangle bearing2 = compass.bearing(); // should be identical
  stub_timer.wait(11);
  uangle bearing3 = compass.bearing(); // should go on to read the next value

  EXPECT_EQ(bearing, 45);
  EXPECT_EQ(bearing2, 45);
  EXPECT_EQ(bearing3, 85);
}

TEST_F(CompassTest, should_reset_if_errors_are_beyond_threshhold) {
  uint8_t readings[] = {50,0,50,0,50,0,0,0,0,0,50,0};
  stub_i2c.set_results(readings,12);
  stub_i2c.set_error_percent (COMPASS_RESET_ERROR_THRESHOLD + 1);
  stub_timer.set_millis(1000*60*30);
  EXPECT_EQ(compass.resets_per_hour(),0);

  uangle bearing = compass.bearing();

  EXPECT_EQ(compass.resets_per_hour(),2);
}

TEST_F(CompassTest, should_return_the_error_percentage) {
  stub_i2c.set_error_percent (55);
  EXPECT_EQ(compass.err_percent(),55);
}

TEST_F(CompassTest, should_return_an_error_reading_if_compass_fails) {
  uint8_t readings[] = {50,0}; // not enough data
  stub_i2c.set_results(readings,2);

  uangle bearing = compass.bearing();

  EXPECT_EQ(bearing, ANGLE_ERROR);
}

TEST_F(CompassTest, should_return_an_error_reading_if_accelerometer_fails) {
  uint8_t readings[] = {50,0,50,0,50,0,0,0}; // not enough data
  stub_i2c.set_results(readings,8);

  uangle bearing = compass.bearing();

  EXPECT_EQ(bearing, ANGLE_ERROR);
}

TEST_F(CompassTest, should_use_the_tilt_to_calculate_the_bearing) {
  uint8_t readings[] = {0,50,0,50,0,50,0,0,0,0,0,50,0,50,0,50,0,50,100,0,200,0,0,0};
  stub_i2c.set_results(readings,24);
  uangle bearing = compass.bearing();
  stub_timer.wait(11);
  uangle bearing2 = compass.bearing();

  EXPECT_EQ(bearing, 45);
  EXPECT_EQ(bearing2, 226);
}

TEST_F(CompassTest, mag_readings_should_use_the_mag_I2C_address) {
  uint8_t readings[] = {150,20,35,40,55,60,70,80,90,10,110,120};
  stub_i2c.set_results(readings,12);

  MagResult result = compass.raw_bearing();

  EXPECT_EQ(stub_i2c.last_address, LIS2MDL_ADDRESS);
}

TEST_F(CompassTest, accel_readings_should_use_the_accel_I2C_address) {
  uint8_t readings[] = {150,20,35,40,55,60,70,80,90,10,110,120};
  stub_i2c.set_results(readings,12);

  MagResult result = compass.raw_accel();

  EXPECT_EQ(stub_i2c.last_address, COMPASS_ACCEL_I2C_ADDRESS);
}


}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
