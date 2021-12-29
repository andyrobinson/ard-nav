#ifndef Compass_h
#define Compass_h

#include "Angle.h"

using namespace Angle;

struct MagResult {
  int x;
  int y;
  int z;
};

class Compass
{
  public:
    virtual uangle bearing() = 0;
    virtual void begin() =0;
    virtual MagResult raw_bearing() = 0;
    virtual MagResult raw_accel() = 0;
    virtual int err_percent() =0;
};
#endif
