#ifndef Rudder_h
#define Rudder_h

#include "Arduino.h"
#include "Angle.h"
#include "MServo.h"

#define RUDDER_MAX_DISPLACEMENT 36 // any more risks servo glitch and causes restarts
#define RUDDER_CENTRE 90
#define RUDDER_CHANNEL 3
#define RUDDER_SPEED 15
#define RUDDER_ACCEL 0

using namespace Angle;

class Rudder
{
  public:
    Rudder();
    Rudder(MServo *servo_controlp);
    void set_position(angle);
    void begin();

  private:
    MServo *servo_control;

};

#endif
