#include "Rudder.h"
#include "Angle.h"

using namespace Angle;

Rudder::Rudder() {}

Rudder::Rudder(MServo *servo_controlp): servo_control(servo_controlp) {}

void Rudder::begin() {
  servo_control->setSpeed(RUDDER_CHANNEL, RUDDER_SPEED);
  servo_control->setAccel(RUDDER_CHANNEL, RUDDER_ACCEL);
}

void Rudder::set_position(angle position) {
  position = constrain(position, (short) -RUDDER_MAX_DISPLACEMENT, (short) RUDDER_MAX_DISPLACEMENT);
  uint16_t servo_0_to_180_angle = RUDDER_CENTRE + position;
  servo_control->write(RUDDER_CHANNEL, servo_0_to_180_angle);
}
