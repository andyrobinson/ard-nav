#ifndef WindSensor_h
#define WindSensor_h

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "Wire.h"

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

class WindSensor {
  public:
    WindSensor();
    angle relative();
    uangle absolute(uangle bearing);
    int err_percent();
  private:
    int errors;
};

#endif
