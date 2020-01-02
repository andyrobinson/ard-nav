#ifndef CompassStub_h
#define CompassStub_h

#include "Angle.h"

using namespace Angle;

class Compass
{
  public:
    Compass();
    uangle bearing();
    void set_bearing(uangle new_bearing);
  private:
    uangle current_bearing;
};

#endif
