#ifndef Sail_h
#define Sail_h

#include "Angle.h"
#include "Servo.h"

#define NO_GO_LIMIT         30
#define ANGLE_OF_ATTACK     30
#define PURE_LIFT_LIMIT     90
#define LIFT_TO_DRAG_LIMIT 140

using namespace Angle;

class Sail
{
  public:
    Sail();
    Sail(Servo *servo);
    void set_position(angle relative_wind);
    angle sail_position(angle relative_wind);

  private:
    short sign(short n);
    short abs1(short n);
    Servo *sail_servo;

};

#endif
