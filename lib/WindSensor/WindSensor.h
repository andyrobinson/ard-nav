
#ifndef WindSensor_h
#define WindSensor_h

#include "Arduino.h"
#include "Angle.h"

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

using namespace Angle;

class WindSensor
{
  public:
    WindSensor();
    angle relative();
    void begin();
};

#endif
