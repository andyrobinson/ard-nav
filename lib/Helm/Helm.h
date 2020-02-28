#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp);
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    angle rudder_position;
    angle old_heading;

    void set_rudder(angle new_position, uangle current_heading);
    angle new_rudder(uangle direction, uangle current_heading);
    bool turning(uangle direction, uangle old_heading, uangle new_heading);

};

#endif
