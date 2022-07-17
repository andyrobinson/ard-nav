#ifndef IHelm_h
#define IHelm_h

#include "Angle.h"
#include "Windrange.h"

using namespace Windrange;

class IHelm
{
  public:
    virtual void steer(uangle direction, long steer_time, windrange range) = 0;
};

#endif
