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
  // Enable the accelerometer
  write8(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
}

MagResult Compass::bearing() {
  Wire.beginTransmission((byte) COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  Wire.endTransmission();
  Wire.requestFrom((byte) COMPASS_COMPASS_I2C_ADDRESS, (byte) 6);

  while (Wire.available() < 6);

  byte xhi = Wire.read();
  byte xlo = Wire.read();
  byte zhi = Wire.read();
  byte zlo = Wire.read();
  byte yhi = Wire.read();
  byte ylo = Wire.read();

  return {hilow_toint(xhi,xlo), hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

MagResult Compass::accel() {

  Wire.beginTransmission((byte) COMPASS_ACCEL_I2C_ADDRESS);
  Wire.write(ACCEL_REGISTER_OUT_X_L_A | 0x80);
  Wire.endTransmission();
  Wire.requestFrom((byte) COMPASS_ACCEL_I2C_ADDRESS, (byte) 6);

  while (Wire.available() < 6);

  byte xlo = Wire.read();
  byte xhi = Wire.read();
  byte ylo = Wire.read();
  byte yhi = Wire.read();
  byte zlo = Wire.read();
  byte zhi = Wire.read();

  return {hilow_toint(xhi,xlo), hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
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

int Compass::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}

byte Compass::read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(address, (byte) 1);

  while (Wire.available() < 1);

  value = Wire.read();
  Wire.endTransmission();

  return value;
}