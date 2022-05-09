#include "Battery.h"

Battery::Battery():readings1(0),average1(0.0) {}

float Battery::lipo1v() {
  int pin_voltage = analogRead(LIPO1);
  float result = 3.3 * ((float) pin_voltage)/MAX_ANALOG;
  return result;
}

float Battery::lipo1avgv() {
  return average1;
}

void Battery::add_reading() {
  float single_value = lipo1v();
  if (readings1 < SAMPLES) readings1++;
  average1 = (average1 * (readings1 - 1))/readings1 + single_value/readings1;
}

