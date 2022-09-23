#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define COMPASS_POWER_PIN   13
#define LIS2MDL_ADDRESS     0x1E
#define LIS2MDL_CFG_REG_A   0x60
#define LIS2MDL_CFG_REG_C   0x62
#define LIS2MDL_OUTX_L_REG  0x68
#define LIS2MDL_STATUS_REG  0x67

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
  write_register_value(LIS2MDL_ADDRESS, LIS2MDL_CFG_REG_A, 0x80);  //0x81 for single mode

  write_register_value(LIS2MDL_ADDRESS, LIS2MDL_CFG_REG_C, 0x10);  //block update mode

}

void loop() {

    Wire.beginTransmission(LIS2MDL_ADDRESS);

    // for single mode
    // Wire.write(LIS2MDL_CFG_REG_A);
    // Wire.write(0x81);

    // there is another possible sequence here
    // write the status reg (0x)
    // read a byte
    // repeat until the byte has the Zyxda bit set

    Wire.write(LIS2MDL_OUTX_L_REG);
    Wire.requestFrom(LIS2MDL_ADDRESS,6);
    Serial.print("reading");
    while (Wire.available() < 6) {
        Serial.print(".");
    }
    byte xlo = Wire.read();
    byte xhi = Wire.read();
    byte ylo = Wire.read();
    byte yhi = Wire.read();
    byte zlo = Wire.read();
    byte zhi = Wire.read();
    Wire.endTransmission();

    Serial.println("");
    Serial.print("(");Serial.print(xhi); Serial.print(",");Serial.print(xlo);Serial.print(") ");
    Serial.print("(");Serial.print(yhi); Serial.print(",");Serial.print(ylo);Serial.print(") ");
    Serial.print("(");Serial.print(zhi); Serial.print(",");Serial.print(zlo);Serial.println(") ");

    int x = hilow_toint(xhi,xlo);
    int y = hilow_toint(yhi,ylo);
    int z = hilow_toint(zhi,zlo);

    short bearing = (360 + (short) round(57.2958 * (atan2((float) y, (float) x)))) % 360;

    Serial.print("calc: "); Serial.print(x); Serial.print(","); Serial.print(y); Serial.print(","); Serial.print(z);
    Serial.print(" [");Serial.print(bearing); Serial.println("]");

    delay(1000);
}
