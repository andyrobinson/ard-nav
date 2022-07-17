#ifndef HelmStub_h
#define HelmStub_h

#include "Angle.h"
#include "Utility.h"
#include "Windrange.h"
#include "IHelm.h"

using namespace Angle;
using namespace Windrange;

class Helm : public IHelm
{
  public:
    Helm();
    virtual void steer(uangle direction, long steer_time, windrange range);
    uangle steering(int index);
    long steer_time(int index);
    windrange wind_range(int index);
    int steering_calls();
    void reset();

  private:
    uangle requested_direction[100];
    long requested_steer_time[100];
    windrange requested_windrange[100];
    int i;

};

#endif
