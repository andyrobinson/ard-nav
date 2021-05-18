#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"
#include "Logger.h"
#include "RotaryPID.h"

#define STEER_INTERVAL 200

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp);
    void steer(uangle direction, long steer_time);

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
    angle new_rudder(uangle direction, uangle current_heading, long steer_interval);
    long rot(uangle direction, uangle current_heading, long steer_interval);
    bool heading_and_turn_ok(uangle direction, uangle old_heading, uangle current_heading, long steer_interval);
    bool not_enough_turn(angle desired_rotation, angle actual_rotation);
    bool too_much_turn(angle desired_rotation, angle actual_rotation);

};

#endif
