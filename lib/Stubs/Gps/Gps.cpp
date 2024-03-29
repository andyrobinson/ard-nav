#include "Gps.h"

Gps::Gps() {}

void Gps::begin() {}

void Gps::data(uint32_t max_millis, gpsResult *result) {
  result->pos = gps_data[current_position].pos;
  result->fix = gps_data[current_position].fix;
  result->mps = gps_data[current_position].mps;
  result->avg_mps = gps_data[current_position].avg_mps;
  result->cog = gps_data[current_position].cog;
  result->unixTime = gps_data[current_position].unixTime;
  result->fpLatitude = gps_data[current_position].fpLatitude;
  result->fpLongitude = gps_data[current_position].fpLongitude;
  if (current_position + 1 < list_size) current_position++;
}

void Gps::set_data(gpsResult *new_data, int size) {
  gps_data = new_data;
  list_size = size;
  current_position = 0;
}
