#include "Navigator.h"
#include "math.h"

using namespace Utility;

Navigator::Navigator() {}

Navigator::Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep):
  tacker(tackerp), gps(gpsp), globe(globep) {}

void Navigator::sailto(position destination) {

  gpsResult current_fix = gps->data(MAX_GPS_WAIT_FOR_FIX);

  while (!arrived(current_fix.pos, destination)) {
    uangle direction = globe->bearing(&current_fix.pos, &destination);
    double unlimited_steer_time_ms = ((globe->distance_between(&current_fix.pos, &destination))/current_fix.mps) * 1000;
    long steer_time = round(max1(MIN_STEER_TIME,min1(unlimited_steer_time_ms/2.0, MAX_STEER_TIME)));
    tacker->steer(direction, steer_time, STEER_CHECK_INTERVAL);
    current_fix = gps->data(MAX_GPS_WAIT_FOR_FIX);
  }
}

bool Navigator::arrived(position current, position destination) {
  double remaining = globe->distance_between(&current, &destination);
  return remaining < (destination.error + current.error);
}