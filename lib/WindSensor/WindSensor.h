#ifndef WindSensor_h
#define WindSensor_h

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "Angle.h"
#include "I2C.h"
#include "math.h"

using namespace Angle;

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

class WindSensor {
  public:
    WindSensor();
    WindSensor(I2C *i2cp);
    angle relative();
    uangle absolute(uangle bearing);
    int err_percent();
  private:
    I2C *i2c;
};

#endif
