#ifndef Sail_h
#define Sail_h

#include "Angle.h"

#define NO_GO_LIMIT         40
#define ANGLE_OF_ATTACK     20
#define PURE_LIFT_LIMIT     90
#define LIFT_TO_DRAG_LIMIT 140

class Sail
{
  public:
    Sail();
    angle sail_position(angle relative_wind);

  private:
    short sign(short n);
    short abs(short n);
};

#endif
