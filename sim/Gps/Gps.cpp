#include "Gps.h"

using namespace Utility;

Gps::Gps(Boat *boatp, Timer *timerp): boat(boatp), timer(timerp), avg_speed(MIN_SPEED) {}

void Gps::data(uint32_t max_millis, gpsResult *result) {
  result->pos.latitude = boat->location().latitude;
  result->pos.longitude = boat->location().longitude;
  result->pos.error = MAX_ACCURACY_METRES;
  result->fix = FIX_DGPS;
  result->unixTime = timer->elapsed() / 1000;

  result->mps = boat->speed();
  avg_speed = (0.9 * avg_speed) + (0.1 * result-> mps); // 10 point moving average
  result->avg_mps = avg_speed;
}
