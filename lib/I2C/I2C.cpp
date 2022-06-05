#include "I2C.h"

I2C::I2C():errors(0) {}

void I2C::begin() {
  Wire.begin();
}

uint8_t I2C::write_register_value(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write((uint8_t) reg);
  Wire.write((uint8_t) value);
  uint8_t result = Wire.endTransmission(false);
  record_errors(result);
  return result;
}

uint8_t I2C::write_register(uint8_t address, uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  uint8_t result = Wire.endTransmission();
  record_errors(result);
  return result;
}

void I2C::requestFrom(uint8_t address, uint8_t num_bytes) {
  Wire.requestFrom(address, num_bytes);
}

bool I2C::wait_for_data(uint8_t num_bytes) {
  long start = millis();
  while (Wire.available() < 6 && ((millis() - start) < I2C_TIMEOUT_MILLIS));
  record_errors(Wire.available() >= num_bytes);
  return Wire.available() >= num_bytes;
}

int I2C::err_percent() {
   return errors/100;
}

void I2C::set_error_percent(int percent) {
    errors = percent * 100;
}

void I2C::record_errors(bool is_error) {
  if (is_error) {
    errors = constrain(errors + 100, 0, 10000);
  }
  else {
    errors = constrain(errors -1, 0, 10000);
  }
}
