
#ifndef WindSensorStub_h
#define WindSensorStub_h

#include "Angle.h"

#define NO_WIND_VALUE -999

using namespace Angle;

class WindSensor
{
  public:
    WindSensor();
    angle relative();
    uangle absolute(uangle bearing);
    void set_relative(angle new_angle);
  private:
    angle current_angle;
};

#endif
