#ifndef WindSensorDmac_h
#define WindSensorDmac_h

#ifdef ARDUINO
#include "Arduino.h"
#include "I2C_DMAC.h"
#endif
#include "WindSensor.h"

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

class WindSensorDmac : public WindSensor {
  public:
    WindSensorDmac();
    angle relative();
    uangle absolute(uangle bearing);
    int err_percent();
    void begin();
  private:
    bool wait_with_timeout(volatile bool *busy, int timeout);
    uint8_t data[2];
    int errors;
};

#endif
