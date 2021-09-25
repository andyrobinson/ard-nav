
#ifndef WindSensorSim_h
#define WindSensorSim_h
#define NO_WIND_VALUE -999

#include "Angle.h"
#include "Boat.h"

using namespace Angle;

class WindSensor
{
  public:
    WindSensor(Boat *boatp);
    angle relative();
    uangle absolute(uangle bearing);

  private:
    Boat *boat;
};

#endif
