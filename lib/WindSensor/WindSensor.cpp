#include "WindSensor.h"

WindSensor::WindSensor() {}

void WindSensor::begin() {
  I2C.begin();
}

angle WindSensor::relative() {
  byte endTransResult;
  angle result = 0;
  uint16_t raw_result = 0;

  I2C.initReadBytes(WINDSENSOR_AS5048B_I2C_ADDRESS, data, sizeof(data));
  I2C.initWriteRegAddr(WINDSENSOR_AS5048B_I2C_ADDRESS, WINDSENSOR_AS5048B_I2C_REGISTER);

  I2C.write();
  wait_with_timeout(&(I2C.writeBusy),20);

  I2C.read();
  wait_with_timeout(&(I2C.readBusy),20);

  if (I2C.writeBusy || I2C.readBusy) {
    return NO_WIND_VALUE;
  } else {

    uint8_t lower6bits = data[0];
    uint8_t upper8bits = data[1];

    raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);
    result = to_angle(360 - round((((float) raw_result)/16383.0) * 360.0) % 360);

    return result;
  }
}

bool WindSensor::wait_with_timeout(volatile bool *busy, int timeout) {
  long start = millis();
  while (*busy && ((millis() - start) < timeout));
  return *busy;
}

uangle WindSensor::absolute(uangle bearing) {
  return uadd(bearing, relative());
}
