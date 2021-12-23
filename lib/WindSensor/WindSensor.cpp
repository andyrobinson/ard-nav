#include "WindSensor.h"

WindSensor::WindSensor() {}

void WindSensor::begin() {
  I2C.begin();
}

angle WindSensor::relative() {
  byte endTransResult;
  angle result = 0;
  uint16_t raw_result = 0;

  //I2C.initReadBytes(WINDSENSOR_AS5048B_I2C_ADDRESS, data, sizeof(data));
  I2C.initWriteRegAddr(WINDSENSOR_AS5048B_I2C_ADDRESS, WINDSENSOR_AS5048B_I2C_REGISTER);
  I2C.write();

  long start = millis();
  while (I2C.writeBusy && ((millis() - start) < 100));

  start = millis();
  I2C.read();
  while (I2C.readBusy && ((millis() - start) < 100));

  if (I2C.writeBusy || I2C.readBusy) {
    return NO_WIND_VALUE;
  } else {

    I2C.readByte(WINDSENSOR_AS5048B_I2C_ADDRESS);                     // Read the WHO_AM_I register
    while(I2C.readBusy);
    byte upper8bits = I2C.getData();

    I2C.readByte(WINDSENSOR_AS5048B_I2C_ADDRESS);                     // Read the WHO_AM_I register
    while(I2C.readBusy);
    byte lower6bits = I2C.getData();

    raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);
    result = to_angle(360 - round((((float) raw_result)/16383.0) * 360.0) % 360);

    return result;
  }
}

uangle WindSensor::absolute(uangle bearing) {
  return uadd(bearing, relative());
}
