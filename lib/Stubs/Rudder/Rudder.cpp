#include "Rudder.h"
#include "Angle.h"

using namespace Angle;

Rudder::Rudder():rudder_positions{0}, current_position(0) {}

void Rudder::set_position(angle position) {
  rudder_positions[current_position++] = position;
}

angle *Rudder::get_positions() {
  return rudder_positions;
}

void Rudder::reset() {
  current_position=0;
  for (int i=0; i < 100; i++) {rudder_positions[i] = 0;}
}
