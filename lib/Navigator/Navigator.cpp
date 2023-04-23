#include "Navigator.h"
#include "math.h"

using namespace Utility;

Navigator::Navigator() {}

Navigator::Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep, Logger *loggerp):
  tacker(tackerp), gps(gpsp), globe(globep), logger(loggerp) {}

void Navigator::sailto(waypoint destination) {

  char logmsg[40];
  sprintf(logmsg, "Nav %s", destination.label); logger->banner(logmsg);
  logger->setdest(destination.label[0]);

  gpsResult current_gps = {{0.0, 0.0, 0.0}, FIX_NONE, 0.0, 0, 0, 0};
  gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);
  // gpsResult current_gps = {{53.44048, -2.27279, 4.42}, FIX_GPS, 1.0, 1659806127};

  while (!arrived(current_gps.pos, destination.pos)) {
    uangle direction = globe->bearing(&current_gps.pos, &destination.pos);
    double distance = constrain(globe->distance_between(&current_gps.pos, &destination.pos),0.0,MAX_DISTANCE);

    sprintf(logmsg, "To %s, %d, %dm", destination.label, direction, (long) round(distance));
    logger->banner(logmsg);

    double unlimited_steer_time_ms = (distance/current_gps.avg_mps) * 1000;
    long steer_time = round(constrain(unlimited_steer_time_ms/2.0, MIN_STEER_TIME,MAX_STEER_TIME));
    tacker->steer(direction, steer_time);
    gps->data(MAX_GPS_WAIT_FOR_FIX, &current_gps);
  }

  sprintf(logmsg, "At %s", destination.label); logger->banner(logmsg);
}

bool Navigator::arrived(position current, position dest_position) {
  double remaining = globe->distance_between(&current, &dest_position);
  return remaining < (dest_position.error + current.error);
}
