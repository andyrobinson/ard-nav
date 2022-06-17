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
  CompassTest() {
  }

  void SetUp() override {
    compass = Compass(&stub_i2c, &stub_timer);
    compass.begin();
    stub_timer.set_millis(100);
  }

};

TEST_F(CompassTest, should_return_a_valid_bearing) {
  uint8_t readings[] = {50,0,50,0,50,0,0,0,0,0,50,0};
  stub_i2c.set_results(readings,12);
  uangle bearing = compass.bearing();
  EXPECT_EQ(bearing, 45);
}


TEST_F(CompassTest, should_cache_the_value_for_10_ms) {
  uint8_t readings[] = {50,0,50,0,50,0,0,0,0,0,50,0,100,0,100,0,100,0,0,0,0,0,50,0};
  stub_i2c.set_results(readings,24);
  uangle bearing = compass.bearing();
  //uangle bearing2 = compass.bearing();
  // wait 10 ms
  //uangle bearing3 = compass.bearing();
  EXPECT_EQ(bearing, 45);
  //EXPECT_EQ(bearing2, 45);
  //EXPECT_EQ(bearing3, 95);
}

// TEST_F(CompassTest, should_cache_the_value_for_10_ms)
// TEST_F(CompassTest, should_reset_if_errors_are_beyond_threshhold)
// TEST_F(CompassTest, should_return_the_error_percentage)
// TEST_F(CompassTest, should_return_the_resets_per_hour)
// TEST_F(CompassTest, should_return_an_error_reading_if_compass_fails)
// TEST_F(CompassTest, should_return_an_error_reading_if_accelerometer_fails)
// TEST_F(CompassTest, should_use_the_tilt_to_calculate_the_bearing)

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
