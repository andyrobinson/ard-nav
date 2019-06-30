
#ifndef WindSensor_h
#define WindSensor_h

#include "Arduino.h"
#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

class WindSensor
{
  public:
    WindSensor();
    int angle();
    void begin();
};

#endif
