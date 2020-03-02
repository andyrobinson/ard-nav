#include "Sail.h"
#include "Angle.h"
#include "Utility.h"
#include "Windsensor.h"

using namespace Angle;
using namespace Utility;

Sail::Sail() {}

Sail::Sail(Servo *servo) {
  sail_servo = servo;
}

void Sail::set_position(angle relative_wind) {
  static angle last_position;
  angle servo_0_to_180_angle;

  if (relative_wind == NO_WIND_VALUE) {
      last_position = 30;
  }
  else {
    last_position = gybe_check(last_position, sail_position(relative_wind));
  }
  if (abs1(last_position) > SERVO_MAX_DISPLACEMENT) {
    last_position = sign(last_position) * SERVO_MAX_DISPLACEMENT;
  }
  servo_0_to_180_angle = SERVO_MAX_DISPLACEMENT - last_position;
  sail_servo->write((int) servo_0_to_180_angle);
}

angle Sail::gybe_check(angle old_position, angle new_position) {
  if(abs1(old_position) < GYBE_CHECK_LIMIT) {
    return new_position; // not running
  }

  if(sign(old_position) == sign(new_position)) {
    return new_position; // no potential gybe
  }

  if(abs1(abs1(old_position) - abs1(clockwise(new_position, 180))) > GYBE_CHECK_MAX_DIFF) {
    return new_position; // too big a diff, so gybe
  }

  return sign(old_position) * FULL_RUN;
}

angle Sail::sail_position(angle relative_wind) {
  angle wind_magnitude = abs1(relative_wind);
  angle position_magnitude = 0;
  if (wind_magnitude <= NO_GO_LIMIT) {
    position_magnitude = 0;
  }
  else if (wind_magnitude <= PURE_LIFT_LIMIT) {
      position_magnitude = wind_magnitude - ANGLE_OF_ATTACK;
  }
  else if (wind_magnitude <= LIFT_TO_DRAG_LIMIT) {
    position_magnitude = 70;
  }
  else {
    position_magnitude = (wind_magnitude / 2);
  }
  return sign(relative_wind) * position_magnitude;
}
