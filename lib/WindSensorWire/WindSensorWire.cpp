#include "WindSensor.h"

// Wire.beginTransmission(address);
// Wire.write(/* byte, string, or buffer with length */);
// int result = Wire.endTransmission(/* boolean selecting to send stop bit or not */);

// int bytesRead = Wire.requestFrom(address, byteCount, stopBitBoolean);
// if(bytesRead == byteCount) {
//     // It worked, but we still need to get the bytes from Wire
//     while(Wire.available()>0) {
//         byte b = Wire.read();
//         // do something with this byte
//     }
// }

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
    return NO_WIND_VALUE;
  } else {
    Wire.requestFrom(WINDSENSOR_AS5048B_I2C_ADDRESS, (uint8_t) 2);
    byte upper8bits = Wire.read();
    byte lower6bits = Wire.read();

    raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);
    result = to_angle(360 - round((((float) raw_result)/16383.0) * 360.0) % 360);

    return result;
  }
}

uangle WindSensorWire::absolute(uangle bearing) {
  return uadd(bearing, relative());
}
