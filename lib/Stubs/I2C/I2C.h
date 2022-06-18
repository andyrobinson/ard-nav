#ifndef I2C_h
#define I2C_h

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
    void set_results(uint8_t *results, int size);
    void set_transmission_result(uint8_t result);

  private:
    int errors;
    uint8_t *result_list;
    int list_size;
    int current_position;
    uint8_t trans_result;
};

#endif
