#include "Arduino.h"
#include "Wire.h"
#include "Compass.h"

Compass::Compass() {

}

void Compass::begin() {
  //Wire.begin();
  Serial.println("initialising compass");
  // Enable the compass
   write8(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
}

int Compass::bearing() {

  int result;

  Wire.beginTransmission((byte)COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  Wire.endTransmission();
  Wire.requestFrom((byte)COMPASS_COMPASS_I2C_ADDRESS, (byte)6);

  while (Wire.available() < 6);

  byte xhi = Wire.read();
  byte xlo = Wire.read();
  byte zhi = Wire.read();
  byte zlo = Wire.read();
  byte yhi = Wire.read();
  byte ylo = Wire.read();

  result = (int16_t)((uint16_t)xlo | ((uint16_t)xhi << 8));
  return result;
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

void Compass::write8(byte address, byte reg, byte value)
{
  byte endTransResult;
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  endTransResult = Wire.endTransmission(false);
  if (endTransResult) {
    Serial.println("ERROR: Write failed " + String(address) + " " + String(reg) + " " + String(value));
  }

}

byte Compass::read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (byte)1);

  while (Wire.available() < 1);

  value = Wire.read();
  Wire.endTransmission();

  return value;
}
