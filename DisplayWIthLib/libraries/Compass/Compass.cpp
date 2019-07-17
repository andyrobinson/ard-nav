#include "Arduino.h"
#include "Wire.h"
#include "Compass.h"

Compass::Compass() {

}

void Compass::begin() {
  Wire.begin();

  write8(COMPASS_COMPASS_I2C_ADDRESS,0, 0b01110000); // Set to 8 samples @ 15Hz
  write8(COMPASS_COMPASS_I2C_ADDRESS,1, 0b00100000); // 1.3 gain LSb / Gauss 1090 (default)
  write8(COMPASS_COMPASS_I2C_ADDRESS,2, 0b00000000); // Continuous sampling
}

int Compass::bearing() {
  int result;
  byte xhi = read8(COMPASS_COMPASS_I2C_ADDRESS,0x03);
  byte xlo = read8(COMPASS_COMPASS_I2C_ADDRESS,0x04);

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
  Wire.beginTransmission(address);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
  Wire.endTransmission();
}

byte Compass::read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (byte)1);
  value = Wire.read();
  Wire.endTransmission();

  return value;
}
