#include "Boat.h"

Boat::Boat(position *start, Logger *loggerp):
  logger(loggerp){
    heading = STARTING_HEADING;
    rudder = 0;
    speed_ms = STARTING_SPEED;
    absolute_wind = STARTING_WIND;
    globe = Globe();
    current_position = {start->latitude, start->longitude, start->error};
  }

position Boat::location() {
  return current_position;
}

double Boat::speed() {
    return speed_ms;
}

angle Boat::relative_wind() {
    return add(absolute_wind, -heading);
}

uangle Boat::bearing() {
    return heading;
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
