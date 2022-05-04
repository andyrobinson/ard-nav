#include "Wire.h"
#include "Compass.h"

Compass::Compass(){}
Compass::Compass(Timer* timerp):errors(0),timer(timerp),reset_pause(COMPASS_INITIAL_RESET_PAUSE_MS),resets_ph(0) {}

void Compass::begin() {
  pinMode(COMPASS_POWER_PIN, OUTPUT);
  digitalWrite(COMPASS_POWER_PIN, HIGH);
  timer->wait(50);

  // Enable the compass
  write8(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  write8(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
  last_read_time = millis() - COMPASS_CACHE_TTL_MS;
  last_reset = millis();
}

uangle Compass::bearing() {

    if ((millis() - last_read_time) < COMPASS_CACHE_TTL_MS) {
      return tiltadjust;
    }

   if (err_percent() >= COMPASS_RESET_ERROR_THRESHOLD) {
     reset();
   }

   if (err_percent() == 0) {
     reset_pause = COMPASS_INITIAL_RESET_PAUSE_MS;
   }

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
   tiltadjust = (360 + (short) round(57.2958 * (atan2(y_final,x_final)))) % 360;

   long last_read_time = millis();

   return tiltadjust;
}

MagResult Compass::raw_bearing() {
  byte endTransResult;
  Wire.beginTransmission((byte) COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
    return {0,0,0};
  }

  Wire.requestFrom((byte) COMPASS_COMPASS_I2C_ADDRESS, (byte) 6);

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

  errors = constrain(errors-1, 0, 10000);

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

MagResult Compass::raw_accel() {
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

  while (Wire.available() < 6);

  byte xlo = Wire.read();
  byte xhi = Wire.read();
  byte ylo = Wire.read();
  byte yhi = Wire.read();
  byte zlo = Wire.read();
  byte zhi = Wire.read();

  errors = constrain(errors-1, 0, 10000);

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
    errors = constrain(errors + 100, 0, 10000);
  }

}

int Compass::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}

int Compass::err_percent() {
   return errors/100;
}

long Compass::resets_per_hour() {
  return resets_ph;
}

void Compass::reset() {
  digitalWrite(COMPASS_POWER_PIN, LOW);
  timer->wait(reset_pause);
  // Seems to recover better WITHOUT a Wire reset
  begin();

  // poor approximation based on last two resets
  // gives maximum of 3600 (one per second), min of zero
  long time_from_last_reset = millis() - last_reset;
  time_from_last_reset = max1(time_from_last_reset, 1000l);
  resets_ph = COMPASS_MILLIS_PER_HOUR/time_from_last_reset;

  // once we have reached the limit, don't increase the pause further or reset errors
  if (reset_pause <= COMPASS_MAX_RESET_PAUSE_MS) {
    errors = 100; // one percent, so that we don't reset the back-off
    reset_pause = reset_pause * 2; // exponential back-off
  }
}
