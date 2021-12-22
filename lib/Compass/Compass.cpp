#include "I2C_DMAC.h"
#include "Compass.h"

Compass::Compass() {}

uint8_t data[6];

void Compass::begin() {

  I2C.begin(400000);                                        // Start I2C bus at 400kHz
  I2C.initReadBytes(COMPASS_COMPASS_I2C_ADDRESS, data, sizeof(data));   // Set DMAC to read the data

  // Enable the compass
  I2C.writeByte(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  I2C.writeByte(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
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
  byte xlo = 0;  byte xhi = 0;  byte ylo = 0;  byte yhi = 0;  byte zlo = 0;  byte zhi = 0;

  I2C.initWriteRegAddr(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_X_HIGH);       // Set-up DMAC to write to MPU6050 register pointer
  I2C.write();                                              // DMAC write to set MPU6050 register pointer to start of the data

  long start = millis();
  while (I2C.writeBusy && ((millis() - start) < 2));

  start = millis();
  I2C.read();
  while (I2C.readBusy && ((millis() - start) < 2));

  if (I2C.writeBusy || I2C.readBusy) {
    Serial.println("raw_bearing ERROR: Busy state while writing/reading");
  } else {
      xhi = data[0];
      xlo = data[1];
      zhi = data[2];
      zlo = data[3];
      yhi = data[4];
      ylo = data[5];
  }

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

MagResult Compass::raw_accel() {
  byte xlo = 0;  byte xhi = 0;  byte ylo = 0;  byte yhi = 0;  byte zlo = 0;  byte zhi = 0;

  I2C.initWriteRegAddr(COMPASS_ACCEL_I2C_ADDRESS, ACCEL_REGISTER_OUT_X_L_A | 0x80);       // Set-up DMAC to write to MPU6050 register pointer
  I2C.write();                                              // DMAC write to set MPU6050 register pointer to start of the data

  long start = millis();
  while (I2C.writeBusy && ((millis() - start) < 2));

  start = millis();
  I2C.read();
  while (I2C.readBusy && ((millis() - start) < 2));

  if (I2C.writeBusy || I2C.readBusy) {
    Serial.println("raw_accel ERROR: Busy state while writing/reading");
  } else {
    xlo = data[0];
    xhi = data[1];
    ylo = data[2];
    yhi = data[3];
    zlo = data[4];
    zhi = data[5];
  }

  return {hilow_toint(xhi,xlo), hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

int Compass::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}
