#ifndef Helm_h
#define Helm_h

#include "IHelm.h"
#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"
#include "Logger.h"
#include "RotaryPID.h"
#include "Windrange.h"

using namespace Windrange;

#define STEER_INTERVAL 300
#define WIND_RANGE_GRACE_PERIOD 10000

class Helm : public IHelm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp);
    virtual void steer(uangle direction, long steer_time, windrange range);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    RotaryPID *rotarypid;
    Logger *logger;
    angle rudder_position;

    bool ok_to_continue(long time_left, long total_time, windrange range);
};

#endif
