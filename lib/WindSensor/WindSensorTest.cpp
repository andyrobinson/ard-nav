#include "WindSensor.h"
#include "I2C.h"
#include "Angle.h"
#include "gtest/gtest.h"

namespace {

I2C stub_i2c;
WindSensor sensor;

class WindSensorTest : public ::testing::Test {
 protected:
  WindSensorTest() {}

  void SetUp() override {
    sensor = WindSensor(&stub_i2c);
  }
};

TEST_F(WindSensorTest, should_return_a_valid_relative_wind_direction) {
  uint8_t readings[] = {0x80,0x00, 0x40, 0x00, 0xC0,0x00};
  stub_i2c.set_results(readings,6);
  angle wind = sensor.relative();
  angle wind2 = sensor.relative();
  angle wind3 = sensor.relative();

  EXPECT_EQ(wind, 180);
  EXPECT_EQ(wind2, -90);
  EXPECT_EQ(wind3, 90);
}

TEST_F(WindSensorTest, should_return_an_absolute_value_based_on_the_compass_bearing) {
  uint8_t readings[] = {0x80,0x00};
  stub_i2c.set_results(readings,2);
  angle wind = sensor.absolute(110);

  EXPECT_EQ(wind, 290);
}

TEST_F(WindSensorTest, should_return_an_error_if_I2C_returns_a_write_error) {
  uint8_t readings[] = {0x80,0x00};
  stub_i2c.set_results(readings,2);
  stub_i2c.set_transmission_result(1);
  angle wind = sensor.relative();

  EXPECT_EQ(wind, ANGLE_ERROR);
}

TEST_F(WindSensorTest, should_return_an_error_if_I2C_returns_a_read_error) {
  uint8_t readings[] = {0x80};
  stub_i2c.set_results(readings,1);
  stub_i2c.set_transmission_result(0);
  angle wind = sensor.relative();

  EXPECT_EQ(wind, ANGLE_ERROR);
}

TEST_F(WindSensorTest, should_pass_though_the_error_percentage) {
  stub_i2c.set_error_percent(36);
  EXPECT_EQ(sensor.err_percent(),36);
}

}  //namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
