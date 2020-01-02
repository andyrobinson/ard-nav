#include "Compass.h"

Compass::Compass() {
  current_bearing = 0;
}

uangle Compass::bearing() {
   return current_bearing;
}

void Compass::set_bearing(uangle new_bearing) {
  current_bearing = new_bearing;
}
