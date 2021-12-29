#include "I2C_DMAC.h"
#include "CompassDmac.h"

CompassDmac::CompassDmac(): errors(0) {}

void CompassDmac::begin() {
  I2C.begin(400000);

  // Enable the compass
  I2C.writeByte(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  I2C.writeByte(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);
}

uangle CompassDmac::bearing() {

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

MagResult CompassDmac::raw_bearing() {
  for (int i = 0; i < 6; i++) {data[i] = 0;}

  I2C.initReadBytes(COMPASS_COMPASS_I2C_ADDRESS, data, sizeof(data));
  I2C.initWriteRegAddr(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_X_HIGH);       // Set-up DMAC to write to MPU6050 register pointer

  I2C.write();
  wait_with_timeout(&(I2C.writeBusy),200);

  I2C.read();
  wait_with_timeout(&(I2C.readBusy),200);

  if (I2C.writeBusy || I2C.readBusy) {
    errors = constrain(errors + 100, 0, 10000);
  } else {
    errors = constrain(errors-1, 0, 10000);
  }

  return {hilow_toint(data[0],data[1]) + COMPASS_X_CORRECTION, hilow_toint(data[4],data[5]), hilow_toint(data[2],data[3])};
}

MagResult CompassDmac::raw_accel() {
  for (int i = 0; i < 6; i++) {data[i] = 0;}

  I2C.initReadBytes(COMPASS_COMPASS_I2C_ADDRESS, data, sizeof(data));
  I2C.initWriteRegAddr(COMPASS_ACCEL_I2C_ADDRESS, ACCEL_REGISTER_OUT_X_L_A | 0x80);
  I2C.write();
  wait_with_timeout(&(I2C.writeBusy),200);

  I2C.read();
  wait_with_timeout(&(I2C.readBusy),200);

  if (I2C.writeBusy || I2C.readBusy) {
    errors = constrain(errors + 100, 0, 10000);
  } else {
    errors = constrain(errors-1, 0, 10000);
  }

  return {hilow_toint(data[1],data[0]), hilow_toint(data[3],data[2]), hilow_toint(data[5],data[4])};
}

bool CompassDmac::wait_with_timeout(volatile bool *busy, int timeout) {
  long start = millis();
  while (*busy && ((millis() - start) < timeout));
  return *busy;
}

int CompassDmac::err_percent() {
  return errors;
}

int CompassDmac::hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}
