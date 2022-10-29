#include "Boat.h"

Boat::Boat(position *start):
    heading(STARTING_HEADING),rudder(90),speed_ms(STARTING_SPEED),absolute_wind(STARTING_WIND),globe(Globe()),
    current_position({start->latitude, start->longitude, start->error}),sail(90) {}

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

void Boat::setLogger(Logger *loggerp) {
    logger = loggerp;
}

void Boat::move(unsigned long milliseconds) {

  // speed should use this wind factor =(MAX((190-X*0.7)  -EXP((190-X*0.7)*0.03),0))/90
  // where X is the ABS(relative wind)
  // this should give a zero value up to 30, then maxes out at around 90 or 100
  for (long t=100; t < milliseconds; t+=100) {
    double distance = speed_ms * ((double) t) / 1000.0;
    heading = new_heading(t);
    current_position = globe.new_position(&current_position, heading, distance);
  }
}

uangle Boat::new_heading(unsigned long milliseconds) {
  // TODO: make speed of rotation depend upon speed
  // TODO: add angular momentum, with short decay
  // Note that rudder deflection is from 0 to 180, with 90 in the middle

  angle delta = - (angle)(((double) (90 - rudder) * milliseconds) / 1000.0);
  return uadd(heading, delta);
}
