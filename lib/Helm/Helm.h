#ifndef Helm_h
#define Helm_h

#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"

class Helm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp);
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    angle rudder_position;
    angle old_heading;

    bool turning(uangle direction, uangle old_heading, uangle new_heading);

};

#endif
