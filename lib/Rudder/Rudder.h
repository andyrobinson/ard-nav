#ifndef Sail_h
#define Sail_h

#include "Angle.h"
#include "Servo.h"

#define RUDDER_MAX_DISPLACEMENT 45
#define RUDDER_CENTRE 90

using namespace Angle;

class Rudder
{
  public:
    Rudder();
    Rudder(Servo *servo);
    void set_position(angle);

  private:
    Servo *rudder_servo;

};

#endif
