#include "Wire.h"
#include "CompassWire.h"

CompassWire::CompassWire(): errors(0) {}

void CompassWire::begin() {
  //Wire.begin();

  // Enable the compass
  write8(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  write8(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
}

uangle CompassWire::bearing() {
   MagResult bearing = raw_bearing();
   MagResult accel = raw_accel();

   int heading = (360 + round(57.2958 * atan2((double) bearing.y, (double) bearing.x))) % 360;

   double roll = atan2((double)accel.y, (double)accel.z);
   double pitch = atan2((double) -accel.x, (double) accel.z); // reversing x accel makes it work
   double sin_roll = sin(roll);
   double cos_roll = cos(roll);
   double cos_pitch = cos(pitch);
   double sin_pitch = sin(pitch);

   double x_final = ((double) bearing.x) * cos_pitch + ((double) bearing.y)*sin_roll*sin_pitch+((double) bearing.z)*cos_roll*sin_pitch;
   double y_final = ((double) bearing.y) * cos_roll-((double) bearing.z) * sin_roll;
   uangle tiltadjust = (360 + round(57.2958 * (atan2(y_final,x_final)))) % 360;

   return tiltadjust;
}

MagResult CompassWire::raw_bearing() {
  byte endTransResult;

  Wire.beginTransmission((byte) COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
    return {0,0,0};
  }

  // TODO: If this times out then we will get all zeros
  // We need to check for the timeout and act accordingly
  Wire.requestFrom((byte) COMPASS_ACCEL_I2C_ADDRESS, (byte) 6);

  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < 20));

  if (Wire.available() < 6) {
    errors = constrain(errors + 100, 0, 10000);
    return {0,0,0};
  }

  byte xhi = Wire.read();
  byte xlo = Wire.read();
  byte zhi = Wire.read();
  byte zlo = Wire.read();
  byte yhi = Wire.read();
  byte ylo = Wire.read();

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

MagResult CompassWire::raw_accel() {

  byte endTransResult;

  Wire.beginTransmission((byte) COMPASS_ACCEL_I2C_ADDRESS);
  Wire.write(ACCEL_REGISTER_OUT_X_L_A | 0x80);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
    return {0,0,0};
  }

  Wire.requestFrom((byte) COMPASS_ACCEL_I2C_ADDRESS, (byte) 6);

  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < 20));

  if (Wire.available() < 6) {
    errors = constrain(errors + 100, 0, 10000);
    return {0,0,0};
  }

  errors = constrain(errors -1, 0, 10000);
  byte xlo = Wire.read();
  byte xhi = Wire.read();
  byte ylo = Wire.read();
  byte yhi = Wire.read();
  byte zlo = Wire.read();
  byte zhi = Wire.read();

  return {hilow_toint(xhi,xlo), hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

void CompassWire::write8(byte address, byte reg, byte value)
{
  byte endTransResult;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  endTransResult = Wire.endTransmission(false);

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
  } else {
    errors = constrain(errors -1, 0, 10000);
  }

}

int CompassWire::err_percent() {
   return errors;
 }

int CompassWire::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}
