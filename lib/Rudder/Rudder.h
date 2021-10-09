#ifndef Rudder_h
#define Rudder_h

#include "Angle.h"
#include "VSServoSamd.h"

#define RUDDER_MAX_DISPLACEMENT 45
#define RUDDER_CENTRE 90
#define RUDDER_SPEED 50

using namespace Angle;

class Rudder
{
  public:
    Rudder();
    Rudder(VSServoSamd *servo);
    void set_position(angle);

  private:
    VSServoSamd *rudder_servo;

};

#endif
