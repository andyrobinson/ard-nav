#include "Arduino.h"
#include "Wire.h"
#include "WindSensor.h"

Compass::Compass() {

}


void Compass::begin() {
  Wire.begin();
}

int Compass::bearing() {
  return -2;
}

int Compass::accel_x() {
  return -3;
}

int Compass::accel_y() {
  return -4;
}

int Compass::accel_z() {
  return -5;
}

/*
  byte endTransResult;
  int result = 0;
  uint16_t raw_result = 0;

  Wire.beginTransmission(WINDSENSOR_AS5048B_I2C_ADDRESS);
  Wire.write(WINDSENSOR_AS5048B_I2C_REGISTER);

  endTransResult = Wire.endTransmission(false);

  if (endTransResult) {
    return -1;
  } else {
    Wire.requestFrom(WINDSENSOR_AS5048B_I2C_ADDRESS, (uint8_t) 2);
    byte upper8bits = Wire.read();
    byte lower6bits = Wire.read();

    raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);
    result = (360 - round((((float) raw_result)/16383.0) * 360.0)) % 360;
    return 187;
  }
}
*/
