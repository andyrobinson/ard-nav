#include "WindSensor.h"
#include <iostream>

WindSensor::WindSensor() {}
WindSensor::WindSensor(I2C *i2cp):i2c(i2cp) {}

angle WindSensor::relative() {
  uint8_t endTransResult;
  angle result = 0;
  uint16_t raw_result = 0;

  endTransResult = i2c->write_register(WINDSENSOR_AS5048B_I2C_ADDRESS, WINDSENSOR_AS5048B_I2C_REGISTER);
  if (endTransResult) return ANGLE_ERROR;

  i2c->requestFrom(WINDSENSOR_AS5048B_I2C_ADDRESS, 2);
  if (!i2c->wait_for_data(2)) return ANGLE_ERROR;

  uint8_t upper8bits = i2c->read();
  uint8_t lower6bits = i2c->read();

  raw_result = (((uint16_t) upper8bits) << 6) + (uint16_t) (lower6bits & 0x3F);
  result = to_angle(360 - (short) round((((float) raw_result)/16383.0) * 360.0) % 360);

  return result;
}

int WindSensor::err_percent() {
  return i2c->err_percent();
}

uangle WindSensor::absolute(uangle bearing) {
  angle rel = relative();
  return (rel == ANGLE_ERROR || bearing == ANGLE_ERROR) ? ANGLE_ERROR : uadd(bearing, rel);
}
