#ifndef Tacker_h
#define Tacker_h

#include "Helm.h"
#include "Compass.h"
#include "WindSensor.h"
#include "Angle.h"
#include "Logger.h"
#include "Windrange.h"

using namespace Angle;

#define TACKER_NO_GO_LIMIT 40
#define MIN_TACK_MS 5000

class Tacker
{
  public:
    Tacker();
    Tacker(Helm *helmp, Compass *compassp, WindSensor *windsensorp, Logger *loggerp);
    void steer(uangle direction, long steer_time);

  private:
    Helm *helm;
    Compass *compass;
    WindSensor *windsensor;
    Logger *logger;
};

#endif
