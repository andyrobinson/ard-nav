#include "Gps.h"

Gps::Gps() {}

void Gps::begin() {}

gpsResult Gps::data(uint32_t max_millis) {
  gpsResult result = gps_data[current_position];
  if (current_position + 1 < list_size) current_position++;
  return result;
}

void Gps::set_data(gpsResult *new_data, int size) {
  gps_data = new_data;
  list_size = size;
  current_position = 0;
}
