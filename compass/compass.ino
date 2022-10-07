#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define COMPASS_POWER_PIN   13
#define LIS2MDL_ADDRESS     0x1E
#define LIS2MDL_CFG_REG_A   0x60
#define LIS2MDL_CFG_REG_C   0x62
#define LIS2MDL_OUTX_L_REG  0x68
#define LIS2MDL_STATUS_REG  0x67

#define COMPASS_ACCEL_I2C_ADDRESS      0x19
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20

#define LSM303_REGISTER_ACCEL_OUT_X_L_A 0x28
#define LSM303_REGISTER_ACCEL_OUT_X_H_A 0x29
#define LSM303_REGISTER_ACCEL_OUT_Y_L_A 0x2A
#define LSM303_REGISTER_ACCEL_OUT_Y_H_A 0x2B
#define LSM303_REGISTER_ACCEL_OUT_Z_L_A 0x2C
#define LSM303_REGISTER_ACCEL_OUT_Z_H_A 0x2D


byte read_register(uint8_t address, uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.requestFrom(address,1);
  while (Wire.available() < 1) {}
  return Wire.read();
  return Wire.endTransmission(false);
}

uint8_t write_register_value(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t) reg);
  Wire.write((uint8_t) value);
  return Wire.endTransmission(false);
}

int hilow_toint(uint8_t high, uint8_t low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}

void setup() {
  Wire.begin();
  pinMode(COMPASS_POWER_PIN, OUTPUT);
  digitalWrite(COMPASS_POWER_PIN, HIGH);
  delay(20);  // for boot

  while (!Serial);
  Serial.begin(19200);

  Serial.println("Starting");

  // set up at 10Hz continuous, high resolution
  write_register_value(LIS2MDL_ADDRESS, LIS2MDL_CFG_REG_A, 0x80);
  write_register_value(LIS2MDL_ADDRESS, LIS2MDL_CFG_REG_C, 0x10);  //block update mode

  write_register_value(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);

}

void loop() {

    Wire.beginTransmission(LIS2MDL_ADDRESS);

    Wire.write(LIS2MDL_OUTX_L_REG);
    Wire.requestFrom(LIS2MDL_ADDRESS,6);
    while (Wire.available() < 6) { }

    byte xlo = Wire.read();
    byte xhi = Wire.read();
    byte ylo = Wire.read();
    byte yhi = Wire.read();
    byte zlo = Wire.read();
    byte zhi = Wire.read();
    Wire.endTransmission();

    int x = hilow_toint(xhi,xlo);
    int y = hilow_toint(yhi,ylo);
    int z = hilow_toint(zhi,zlo);

    Wire.endTransmission();

    Wire.beginTransmission(COMPASS_ACCEL_I2C_ADDRESS);

    Wire.write(ACCEL_REGISTER_OUT_X_L_A | 0x80);
    Wire.endTransmission(false);
    Wire.requestFrom(COMPASS_ACCEL_I2C_ADDRESS,6);

    while (Wire.available() < 6) { }

    xlo = Wire.read();
    xhi = Wire.read();
    ylo = Wire.read();
    yhi = Wire.read();
    zlo = Wire.read();
    zhi = Wire.read();

    // xlo = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_X_L_A);
    // xhi = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_X_H_A);
    // ylo = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_Y_L_A);
    // yhi = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_Y_H_A);
    // zlo = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_Z_L_A);
    // zhi = read_register(COMPASS_ACCEL_I2C_ADDRESS,LSM303_REGISTER_ACCEL_OUT_Z_H_A);

    int xacc = hilow_toint(xhi,xlo);
    int yacc = hilow_toint(yhi,ylo);
    int zacc = hilow_toint(zhi,zlo);

    Serial.print(xacc); Serial.print(",");
    Serial.print(yacc); Serial.print(",");
    Serial.print(zacc); Serial.print(" | ");
    // note change in sign because of reversed Y compared to LSM303DLHC
    short bearing = (360 - (short) round(57.2958 * (atan2((float) y, (float) x)))) % 360;

    Serial.println(bearing);

    delay(1000);
}
