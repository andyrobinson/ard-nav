#include <Arduino.h>
#include <Wire.h>

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE
#define NO_WIND_VALUE -1.0;

float bearing() {
  byte endTransResult;
  float result;
  uint16_t raw_result = 0;

  Wire.beginTransmission(WINDSENSOR_AS5048B_I2C_ADDRESS);
  Wire.write(WINDSENSOR_AS5048B_I2C_REGISTER);

  endTransResult = Wire.endTransmission(false);

  if (endTransResult) {
    Serial.println("Failed to read sensor");
    return NO_WIND_VALUE;
  } else {
    Wire.requestFrom(WINDSENSOR_AS5048B_I2C_ADDRESS, (uint8_t) 2);
    byte upper8bits = Wire.read();
    byte lower6bits = Wire.read();

    raw_result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F); // shift upperbits left 6 places, add on lower 6 bits
    result = (((float) raw_result)/16383.0) * 360.0;

    return result;
  }
}

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  Wire.begin();
}

void loop() {
  Serial.println(bearing(),1);
  delay(500); // wait half a second between reads
}
