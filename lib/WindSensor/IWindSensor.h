#ifndef WindSensor_h
#define WindSensor_h

#include "Angle.h"

using namespace Angle;
#define NO_WIND_VALUE -999

class WindSensor
{
  public:
    virtual angle relative() = 0;
    virtual uangle absolute(uangle bearing) = 0;
    virtual int err_percent() = 0;
    virtual void begin() = 0;
};

#endif
