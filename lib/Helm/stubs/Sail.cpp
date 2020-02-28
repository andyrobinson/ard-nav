#include "Sail.h"

using namespace Angle;

Sail::Sail():sail_calls{0}, current_position(0) {}

void Sail::set_position(angle relative_wind) {
  sail_calls[current_position++] = relative_wind;
}

angle* Sail::get_calls() {
  return sail_calls;
}

void Sail::reset() {
  current_position=0;
}
