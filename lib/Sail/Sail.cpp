#include "Sail.h"
#include "Angle.h"

Sail::Sail() {

}

angle Sail::sail_position(angle relative_wind) {
  angle wind_magnitude = abs(relative_wind);
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

short Sail::sign(short n) {
  return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
}

short Sail::abs(short n) {
  return (n >= 0) ? n : -n;
}
