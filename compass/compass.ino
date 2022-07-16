#include <version.h>
#include <wiring_private.h>
#include <Wire.h>

#define MAJOR_VERSION         111
#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19
#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20
#define COMPASS_X_CORRECTION           -100

struct MagResult {
  int x;
  int y;
  int z;
};

char logmsg[50] = "stuff";
MagResult rbearing;
int tol;
int errors;

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

void setup() {
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  while (!Serial);
  Serial.begin(19200);

  Wire.begin();

  // Enable the compass
  write8(COMPASS_COMPASS_I2C_ADDRESS, COMPASS_REGISTER_ENABLE, 0x00);
  // Enable the accelerometer
  write8(COMPASS_ACCEL_I2C_ADDRESS, COMPASS_ACCEL_CTRL_REG1_A, 0x27);

  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  Serial.println(logmsg);
}

MagResult raw_bearing() {
  byte endTransResult;

  Wire.beginTransmission((byte) COMPASS_COMPASS_I2C_ADDRESS);
  Wire.write(COMPASS_REGISTER_X_HIGH);
  endTransResult = Wire.endTransmission();

  if (endTransResult) {
    errors = constrain(errors + 100, 0, 10000);
    Wire.end();
    Wire.begin();
    return {0,0,0};
  }

  Wire.requestFrom((byte) COMPASS_ACCEL_I2C_ADDRESS, (byte) 6);

  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < 20));

  if (Wire.available() < 6) {
    errors = constrain(errors + 100, 0, 10000);
    Wire.end();
    Wire.begin();
    return {0,0,0};
  }

  errors = constrain(errors -1, 0, 10000);

  byte xhi = Wire.read();
  byte xlo = Wire.read();
  byte zhi = Wire.read();
  byte zlo = Wire.read();
  byte yhi = Wire.read();
  byte ylo = Wire.read();

  return {hilow_toint(xhi,xlo) + COMPASS_X_CORRECTION, hilow_toint(yhi,ylo), hilow_toint(zhi,zlo)};
}

void write8(byte address, byte reg, byte value)
{
  byte endTransResult;

  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  endTransResult = Wire.endTransmission(false);

  if (endTransResult) {
    Wire.end();
    Wire.begin();
    errors = constrain(errors + 100, 0, 10000);
  } else {
    errors = constrain(errors -1, 0, 10000);
  }

}

int hilow_toint(byte high, byte low) {
  return (int16_t)((uint16_t) low | ((uint16_t) high << 8));
}

void loop() {

    int TEMP_RUDDER = 25;

    // lots of exercise for I2C
    for (int i=0; i< 200;i++) {
      rbearing = raw_bearing();
      delay(5);
    }

    Serial3.write(TEMP_RUDDER);

    Serial.print(millis()/1000); Serial.print(",(");
    Serial.print(rbearing.x); Serial.print(",");
    Serial.print(rbearing.y); Serial.print(",");
    Serial.print(rbearing.z); Serial.print("),");
    Serial.print(errors); Serial.print(",");
    Serial.print(tol); Serial.print(",");
    Serial.println("logging");

    if (errors >= 10000) {
      Serial.println("** I2C Failure **");
      while (true) {};
    }

    TEMP_RUDDER = -TEMP_RUDDER;

}
