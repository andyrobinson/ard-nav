#ifndef CompassSim_h
#define CompassSim_h

#include "Angle.h"
#include "Boat.h"

using namespace Angle;

struct MagResult {
  int x;
  int y;
  int z;
};

class Compass
{
  public:
    Compass(Boat *boatp);
    uangle bearing();
    int err_percent();

  private:
  Boat *boat;

};

#endif
