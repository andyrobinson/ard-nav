#ifndef I2C_h
#define I2C_h

#include "Arduino.h"
#include "Wire.h"

#define I2C_TIMEOUT_MILLIS 20

class I2C
{
  public:
    I2C();
    void begin();
    uint8_t write_register_value(uint8_t address, uint8_t reg, uint8_t value);
    uint8_t write_register(uint8_t address, uint8_t reg);
    void requestFrom(uint8_t address, uint8_t num_bytes);
    bool wait_for_data(uint8_t num_bytes);
    uint8_t read();
    int err_percent();
    void set_error_percent(int percent);

  private:
    void record_errors(bool is_error);
    int errors;

};

#endif
