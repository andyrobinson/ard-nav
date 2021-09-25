#include "Rudder.h"
#include "Angle.h"

using namespace Angle;

Rudder::Rudder() {}

Rudder::Rudder(Boat *boatp) {
  boat = boatp;
}

void Rudder::set_position(angle position) {

  if (position > RUDDER_MAX_DISPLACEMENT) {
    boat->rudder = RUDDER_MAX_DISPLACEMENT;
  }
  else if (position < -RUDDER_MAX_DISPLACEMENT) {
    boat->rudder = -RUDDER_MAX_DISPLACEMENT;
  }
  else {
    boat->rudder = position;
  }

}
