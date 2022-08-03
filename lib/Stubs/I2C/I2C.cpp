#include "I2C.h"

I2C::I2C():errors(0),trans_result(0) {}

void I2C::begin() {}

uint8_t I2C::write_register_value(uint8_t address, uint8_t reg, uint8_t value) {
  last_address = address;
  return trans_result;
}

uint8_t I2C::write_register(uint8_t address, uint8_t reg) {
  last_address = address;
  return trans_result;
}

void I2C::requestFrom(uint8_t address, uint8_t num_bytes) {
  last_address = address;
}

bool I2C::wait_for_data(uint8_t num_bytes) {
  return (list_size - current_position) >= num_bytes;
}

uint8_t I2C::read() {
  uint8_t value = result_list[current_position++];
  current_position = current_position >= list_size ? list_size - 1 : current_position;
  return value;
}

int I2C::err_percent() {
   return errors/100;
}

void I2C::set_error_percent(int percent) {
    errors = percent * 100;
}

void I2C::set_results(uint8_t *results, int size) {
  result_list = results;
  list_size = size;
  current_position = 0;
}

void I2C::set_transmission_result(uint8_t result) {
  trans_result = result;
}
