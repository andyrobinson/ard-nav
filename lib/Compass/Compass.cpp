#include "Wire.h"
#include "Compass.h"

Compass::Compass() {}

void Compass::begin() {
  Wire.begin();

  // Enable the compass
  write8(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  write8(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
}

uangle Compass::bearing() {
   MagResult bearing = raw_bearing();
   MagResult accel = raw_accel();


   double roll = atan2((double)accel.y, (double)accel.z);
   double pitch = atan2((double) -accel.x, (double) accel.z); // reversing x accel makes it work
   double sin_roll = sin(roll);
   double cos_roll = cos(roll);
   double cos_pitch = cos(pitch);
   double sin_pitch = sin(pitch);

   double x_final = ((double) bearing.x) * cos_pitch + ((double) bearing.y)*sin_roll*sin_pitch+((double) bearing.z)*cos_roll*sin_pitch;
   double y_final = ((double) bearing.y) * cos_roll-((double) bearing.z) * sin_roll;
   uangle tiltadjust = (360 + (short) round(57.2958 * (atan2(y_final,x_final)))) % 360;

   return tiltadjust;
}

MagResult Compass::raw_bearing() {
  byte endTransResult;
  byte xlo = 0;  byte xhi = 0;  byte ylo = 0;  byte yhi = 0;  byte zlo = 0;  byte zhi = 0;

  Wire.beginTransmission((byte) COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    Serial.println("raw_bearing ERROR: " + String(endTransResult) + " Write failed to " + String(COMPASS_COMPASS_I2C_ADDRESS) + " " + String(COMPASS_REGISTER_X_HIGH));
  }

  Wire.requestFrom((byte) COMPASS_COMPASS_I2C_ADDRESS, (byte) 6);

  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < 2));

  if (Wire.available() < 6) {
    Serial.println("raw_bearing ERROR: Not enough data, wanted 6 got " + String(Wire.available()));
  } else {
      xhi = Wire.read();
      xlo = Wire.read();
      zhi = Wire.read();
      zlo = Wire.read();
      yhi = Wire.read();
      ylo = Wire.read();
  }

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

MagResult Compass::raw_accel() {
  byte endTransResult;
  byte xlo = 0;  byte xhi = 0;  byte ylo = 0;  byte yhi = 0;  byte zlo = 0;  byte zhi = 0;

  Wire.beginTransmission((byte) COMPASS_ACCEL_I2C_ADDRESS);
  Wire.write(ACCEL_REGISTER_OUT_X_L_A | 0x80);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    Serial.println("raw accel ERROR: " + String(endTransResult) + " Write failed to " + String(ACCEL_REGISTER_OUT_X_L_A) + " 0x80 ");
  }

  Wire.requestFrom((byte) COMPASS_ACCEL_I2C_ADDRESS, (byte) 6);

  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < 2));

  if (Wire.available() < 6) {
    Serial.println("raw accel ERROR: Not enough data, wanted 6 got " + String(Wire.available()));
  } else {
    xlo = Wire.read();
    xhi = Wire.read();
    ylo = Wire.read();
    yhi = Wire.read();
    zlo = Wire.read();
    zhi = Wire.read();
  }

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
    Serial.println("write8 ERROR: Write failed " + String(address) + " " + String(reg) + " " + String(value));
  }

}

int Compass::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}
