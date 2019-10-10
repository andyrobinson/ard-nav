#include "Sail.h"

Sail::Sail() {

}

int Sail::sail_position(int relative_wind) {
  int wind_magnitude = abs(relative_wind);
  int position_magnitude = 0;
  if (wind_magnitude <=40) {
    position_magnitude = 0;
  }
  else if (wind_magnitude <= 90) {
      position_magnitude = wind_magnitude - 20;
  }
  else if (wind_magnitude <= 140) {
    position_magnitude = 70;
  }
  else {
    position_magnitude = (wind_magnitude / 2);
  }
  return sign(relative_wind) * position_magnitude;
}

int Sail::sign(int n) {
  return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
}

int Sail::abs(int n) {
  return (n >= 0) ? n : -n;
}
