#ifndef TankHelm_h
#define TankHelm_h

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

#define STEER_INTERVAL 800
#define RUDDER_VALUE 30
#define SAIL_MAX 60

class TankHelm : public IHelm
{
  public:
    TankHelm();
    TankHelm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp);
    virtual  steer(uangle direction, long steer_time, windrange range);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    RotaryPID *rotarypid;
    Logger *logger;
    angle rudder_position;
    uangle old_heading;

    void set_rudder(angle new_position, uangle current_heading);
    long rot(uangle direction, uangle current_heading, long steer_interval);

};

#endif
