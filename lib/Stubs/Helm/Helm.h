#ifndef HelmStub_h
#define HelmStub_h

#include "Angle.h"
#include "Utility.h"
using namespace Angle;

class Helm
{
  public:
    Helm();
    void steer(uangle direction, long steer_time);
    uangle steering(int index);
    long steer_time(int index);
    long interval(int index);
    int steering_calls();
    void reset();

  private:
    uangle requested_direction[100];
    long requested_steer_time[100];
    int i;

};

#endif
