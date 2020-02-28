#ifndef HelmStub_h
#define HelmStub_h

#include "Angle.h"
#include "Utility.h"
using namespace Angle;

class Helm
{
  public:
    Helm();
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);
    uangle steering();
    long steer_time();
    long interval();

  private:
    uangle requested_direction;
    long requested_steer_time;
    long requested_interval;

};

#endif
