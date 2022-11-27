  #include "Boat.h"

Boat::Boat(position *start, double start_speed):
    heading(STARTING_HEADING),rudder(90),speed_ms(start_speed),absolute_wind(STARTING_WIND),globe(Globe()),
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
  for (long t=0; t < milliseconds; t+=TIME_INCREMENT) {
    double distance = speed_ms * ((double) TIME_INCREMENT) / 1000.0;
    speed_ms = new_speed(speed_ms, sail_force(), drag(speed_ms), TIME_INCREMENT);
    heading = new_heading(TIME_INCREMENT);
    current_position = globe.new_position(&current_position, heading, distance);
  }
}

double Boat::new_speed(double speed, double impetus, double drag, long millis) {
  double accel_mss = (impetus-drag) / BOAT_MASS_KG;
  return speed + (accel_mss * ((double) millis)/1000.0);
}

double Boat::sail_force() {
  // forces due to drag
  // f = 0.5 * p * v ^2 * A
  // p = density of air = 1.2
  // v = wind speed m/s
  // A = area of surface
  // ignored the drag coefficient (i.e. flat surface)

  // Area of sail = 0.14m ^ 2

  // example 10m/s wind from behind (=20mph) produces a force of 8.4 newtons.  With no drag this produces an acceleration of 0.62 m/s
  // a 20m/s wind (=40mph) produces a force of 33.6 newtons
  // These are good values for stress testing the sail

  // forces due to lift
  // TBC

  // currently just a constant
  return 8.0;
}

// Mass of boat = 13.4kg
// 1 mph = 0.447 m/s


double Boat::drag(double speed) {
  // for the hull the calculation is
  // f = 0.5 * Cd * p * A * v^2
  // Cd = drag coefficient (somewhere between 0.01 and 0.1, let's say 0.1)
  // p = density of water = 1000
  // A = cross sectional Area of boat = 0.04m^2
  // v = velocity

  // or combining all the constants simply 2 * v^2
  // example travelling at 0.5 m/s produces a drag of 0.5 newtons.  Travelling at 1m/s produces a drag of 2 newtons
  // At low speeds this formula should work, but at higher speeds the bow and stern wave come into effect.  Thus
  // HULL_DRAG_CONSTANT = HULL_SPEED_MS * 1.1 * [combined constants] = 1.32 * 2 = 2.6
  //
  // we also limit speed in this calculation to avoid possible negative numbers or overflow

    return (HULL_DRAG_CONSTANT * speed * speed)/(HULL_SPEED_MS * 1.1 - min1(speed,HULL_SPEED_MS * 1.099));
}

uangle Boat::new_heading(unsigned long milliseconds) {
  // TODO: add angular momentum, with short decay
  // Note that rudder deflection is -90 to + 90, but the heading movement is opposite

  angle delta = - (angle) round((speed_ms * ((double) rudder * milliseconds)) / 1000.0);
  return uadd(heading, delta);
}
