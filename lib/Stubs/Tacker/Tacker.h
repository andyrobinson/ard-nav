#ifndef TackerStub_h
#define TackerStub_h

#include "Angle.h"
using namespace Angle;

class Tacker
{
  public:
    Tacker();
    void steer(uangle direction, unsigned long steer_time, unsigned long steer_interval);
    uangle steering(int index);
    long steer_time(int index);
    long interval(int index);
    bool never_called();
    void reset();

  private:
    uangle requested_direction[100];
    long requested_steer_time[100];
    long requested_interval[100];
    int i;
};

#endif
