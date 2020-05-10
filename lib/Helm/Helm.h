#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"
#include "Logger.h"

#define MIN_DIFF_DEGREES  3
#define NUDGE_DEGREES 4

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, Logger *loggerp);
    void steer(uangle direction, unsigned long steer_time, long steer_interval);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    Logger *logger;
    angle rudder_position;
    uangle old_heading;

    void set_rudder(angle new_position, uangle current_heading);
    angle new_rudder(uangle direction, uangle current_heading, long steer_interval);
    long rot(uangle direction, uangle current_heading, long steer_interval);
    bool heading_and_turn_ok(uangle direction, uangle old_heading, uangle current_heading);

};

#endif
