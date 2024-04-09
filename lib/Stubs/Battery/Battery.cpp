#include "Battery.h"

Battery::Battery() {}

float Battery::lipomaxv(uint8_t batt) {
  return to_volts(raw_max(batt));
}

float Battery::lipominv(uint8_t batt) {
  return to_volts(raw_min(batt));
}

uint16_t Battery::raw_min(uint8_t batt) {
  return rmin[batt];
}

uint16_t Battery::raw_max(uint8_t batt) {
  return rmax[batt];
}

void Battery::add_reading() {}

void Battery::setMaxMin(uint8_t batt, uint16_t max, uint16_t min) {
    rmax[batt] = max;
    rmin[batt] = min;
}

float Battery::to_volts(uint16_t reading) {
  float result = 6.6 * ((float) reading)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;
}
