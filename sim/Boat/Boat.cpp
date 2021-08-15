#include "Boat.h"

Boat::Boat(position *start, Logger *loggerp):
  logger(loggerp){
    heading = 0;
    rudder = 0;
    speed_ms = 1.0;
    globe = Globe();
    current_position = {start->latitude, start->longitude, start->error};
  }

position Boat::location() {
  return current_position;
}

void Boat::move(unsigned long milliseconds) {
  double distance = speed_ms * ((double) milliseconds) / 1000.0;
  heading = new_heading(milliseconds);
  current_position = globe.new_position(&current_position, heading, distance);
}

uangle Boat::new_heading(unsigned long milliseconds) {
  // TODO: make speed of rotation depend upon speed
  // TODO: add angular momentum, with short decay
  angle delta = - (angle)(((double) rudder * milliseconds) / 1000.0);
  return uadd(heading, delta);
}
