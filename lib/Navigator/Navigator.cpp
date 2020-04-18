#include "Navigator.h"
#include "math.h"

using namespace Utility;

Navigator::Navigator() {}

Navigator::Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep, Logger *loggerp):
  tacker(tackerp), gps(gpsp), globe(globep), logger(loggerp) {}

void Navigator::sailto(waypoint destination) {

  char logmsg[30];
  sprintf(logmsg, "** Navigating to %s", destination.label);
  logger->msg(logmsg);

  static gpsResult current_gps;
  gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);

  while (!arrived(current_gps.pos, destination.pos)) {
    uangle direction = globe->bearing(&current_gps.pos, &destination.pos);
    double unlimited_steer_time_ms = ((globe->distance_between(&current_gps.pos, &destination.pos))/current_gps.mps) * 1000;
    long steer_time = round(max1(MIN_STEER_TIME,min1(unlimited_steer_time_ms/2.0, MAX_STEER_TIME)));
    tacker->steer(direction, steer_time, STEER_CHECK_INTERVAL);
    gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);
  }

  sprintf(logmsg, "** Arrived at ", destination.label);
  logger->msg(logmsg);
}

bool Navigator::arrived(position current, position dest_position) {
  double remaining = globe->distance_between(&current, &dest_position);
  return remaining < (dest_position.error + current.error);
}
