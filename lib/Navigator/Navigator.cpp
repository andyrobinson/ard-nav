#include "Navigator.h"
#include "math.h"

using namespace Utility;

Navigator::Navigator() {}

Navigator::Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep):
  tacker(tackerp), gps(gpsp), globe(globep) {}

void Navigator::sailto(position destination) {
  static gpsResult current_gps;
  gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);

  while (!arrived(current_gps.pos, destination)) {
    uangle direction = globe->bearing(&current_gps.pos, &destination);
    double unlimited_steer_time_ms = ((globe->distance_between(&current_gps.pos, &destination))/current_gps.mps) * 1000;
    long steer_time = round(max1(MIN_STEER_TIME,min1(unlimited_steer_time_ms/2.0, MAX_STEER_TIME)));
    tacker->steer(direction, steer_time, STEER_CHECK_INTERVAL);
    gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);
  }
}

bool Navigator::arrived(position current, position destination) {
  double remaining = globe->distance_between(&current, &destination);
  return remaining < (destination.error + current.error);
}
