#include "I2C.h"

I2C::I2C(){}

void I2C::begin() {
  Wire.begin();
}

uint8_t I2C::write_register_value(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)value);
  return Wire.endTransmission(false);
}

uint8_t I2C::write_register(uint8_t address, uint8_t reg) {
  Wire.beginTransmission((address);
  Wire.write(reg);
  return Wire.endTransmission();
}

void I2C::requestFrom(uint8_t address, uint8_t num_bytes) {
  Wire.requestFrom(address, num_bytes);
}

bool I2C::wait_for_data(num_bytes)() {
  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < I2C_TIMEOUT_MILLIS));
  return Wire.available() >= num_bytes;
}
