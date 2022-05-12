#include "Battery.h"

Battery::Battery():readings1(0),average1(0.0) {}

float Battery::lipo1v() {
  int pin_voltage = analogRead(LIPO1);
  float result = 6.6 * ((float) pin_voltage)/MAX_ANALOG;
  add_reading(result);
  return result;
}

float Battery::lipo1avgv() {
  lipo1v();
  return average1;
}

void Battery::add_reading(float battery_volts) {
  if (readings1 < SAMPLES) readings1++;
  average1 = (average1 * (readings1 - 1))/readings1 + battery_volts/readings1;
}

