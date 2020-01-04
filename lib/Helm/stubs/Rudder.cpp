#include "Rudder.h"
#include "Angle.h"

using namespace Angle;

Rudder::Rudder():rudder_position(0) {}

void Rudder::set_position(angle position) {
  rudder_position = position;
}

angle Rudder::get_position() {
  return rudder_position;
}
