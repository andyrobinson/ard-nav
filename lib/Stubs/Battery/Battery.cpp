#include "Battery.h"

Battery::Battery() {}

float Battery::lipo1maxv() {
  return to_volts(raw_max());
}

float Battery::lipo1minv() {
  return to_volts(raw_min());
}

uint16_t Battery::raw_min() {
  return rmin;
}

uint16_t Battery::raw_max() {
  return rmax;
}

void Battery::add_reading() {}

void Battery::setMaxMin(uint16_t max, uint16_t min) {
    rmax = max;
    rmin = min;
}

float Battery::to_volts(uint16_t reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;
}
