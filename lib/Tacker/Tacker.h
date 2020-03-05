#ifndef Tacker_h
#define Tacker_h

#include "Helm.h"
#include "Compass.h"
#include "WindSensor.h"
#include "Angle.h"

using namespace Angle;

#define TACKER_NO_GO_LIMIT 45

class Tacker
{
  public:
    Tacker();
    Tacker(Helm *helmp, Compass *compassp, WindSensor *windsensorp);
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);

  private:
    Helm *helm;
    Compass *compass;
    WindSensor *windsensor;
};

#endif
