#ifndef Tacker_h
#define Tacker_h

#include "Helm.h"
#include "WindSensor.h"
#include "Angle.h"
#include "Logger.h"
#include "Windrange.h"

using namespace Angle;
using namespace Windrange;

#define TACKER_NO_GO_LIMIT 40
#define WIND_RANGE_NO_GO_LIMIT 35 // give ourselves a little lee way, literally
#define MIN_TACK_MS 5000

class Tacker
{
  public:
    Tacker();
    Tacker(Helm *helmp, WindSensor *windsensorp, Logger *loggerp);
    void steer(uangle direction, long steer_time);

  private:
    Helm *helm;
    WindSensor *windsensor;
    Logger *logger;
    windrange set_range(uangle direction, short sign);
};

#endif
