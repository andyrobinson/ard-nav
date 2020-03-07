#include "Gps.h"

Gps::Gps() {}

void Gps::begin() {}

gpsResult Gps::data(uint32_t max_millis) {
  return gps_data;
}

void Gps::set_data(gpsResult new_data) {
  gps_data = new_data;
}
