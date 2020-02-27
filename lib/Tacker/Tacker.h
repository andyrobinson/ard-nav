#ifndef Tacker_h
#define Tacker_h

#include "Helm.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"

#define TACKER_NO_GO_LIMIT 45

class Tacker
{
  public:
    Tacker();
    Tacker(Helm *helmp, Compass *compassp, Timer *timerp, WindSensor *windsensorp);
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);

  private:
    Helm *helm;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
};

#endif
