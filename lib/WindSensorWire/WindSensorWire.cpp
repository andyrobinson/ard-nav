#include "WindSensorWire.h"

WindSensorWire::WindSensorWire() {}

void WindSensorWire::begin() {
  Wire.begin();
}

angle WindSensorWire::relative() {
  byte endTransResult;
  angle result = 0;
  uint16_t raw_result = 0;

  Wire.beginTransmission(WINDSENSOR_AS5048B_I2C_ADDRESS);
  Wire.write(WINDSENSOR_AS5048B_I2C_REGISTER);
  endTransResult = Wire.endTransmission(false);

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
    return NO_WIND_VALUE;
  }

  // TODO: If this times out then we will get all zeros
  Wire.requestFrom(WINDSENSOR_AS5048B_I2C_ADDRESS, (uint8_t) 2);

  long start = millis();
  while (Wire.available() < 2 && ((millis() - start) < 20));

  if (Wire.available() < 2) {
    errors = constrain(errors + 100, 0, 10000);
    return NO_WIND_VALUE;
  }

  errors = constrain(errors -1, 0, 10000);

  byte upper8bits = Wire.read();
  byte lower6bits = Wire.read();

  raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);
  result = to_angle(360 - (short) round((((float) raw_result)/16383.0) * 360.0) % 360);

  return result;
}

int WindSensorWire::err_percent() {
  return errors;
}

uangle WindSensorWire::absolute(uangle bearing) {
  return uadd(bearing, relative());
}
