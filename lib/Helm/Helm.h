#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"
#include "Logger.h"

#define TURNING_MIN_DIFF  3

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, Logger *loggerp);
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    Logger *logger;
    angle rudder_position;
    angle old_heading;

    void set_rudder(angle new_position, uangle current_heading);
    angle new_rudder(uangle direction, uangle current_heading);
    bool turning(uangle direction, uangle old_heading, uangle new_heading);

};

#endif
