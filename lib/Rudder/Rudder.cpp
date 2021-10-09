#include "Rudder.h"
#include "Angle.h"

using namespace Angle;

Rudder::Rudder() {}

Rudder::Rudder(VSServoSamd *servo) {
  rudder_servo = servo;
}

void Rudder::set_position(angle position) {

  if (position > RUDDER_MAX_DISPLACEMENT) {
    position = RUDDER_MAX_DISPLACEMENT;
  }
  else if (position < -RUDDER_MAX_DISPLACEMENT) {
    position = -RUDDER_MAX_DISPLACEMENT;
  }

  int servo_0_to_180_angle = RUDDER_CENTRE - position;
  rudder_servo->write(servo_0_to_180_angle, RUDDER_SPEED);
}
