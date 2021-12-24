#ifndef WindSensor_h
#define WindSensor_h

#ifdef ARDUINO
#include "Arduino.h"
#include "I2C_DMAC.h"
#endif
#include "Angle.h"

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE
#define NO_WIND_VALUE -999

using namespace Angle;

class WindSensor
{
  public:
    WindSensor();
    angle relative();
    uangle absolute(uangle bearing);
    void begin();
  private:
    bool wait_with_timeout(volatile bool *busy, int timeout);
    uint8_t data[2];
};

#endif
