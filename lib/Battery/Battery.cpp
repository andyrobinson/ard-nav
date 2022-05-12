#include "Battery.h"

Battery::Battery():readings1(0),average1(0.0) {}

float Battery::lipo1v() {
  float voltage = read_voltage();
  add_reading(voltage);
  return voltage;
}

float Battery::lipo1avgv() {
  add_reading(read_voltage());
  return average1;
}

void Battery::add_reading(float battery_volts) {
  if (readings1 < SAMPLES) readings1++;
  average1 = (average1 * (readings1 - 1))/readings1 + battery_volts/readings1;
}

float Battery::read_voltage() {
  int pin_voltage = analogRead(LIPO1);
  float result = 6.6 * ((float) pin_voltage)/MAX_ANALOG; // we have divided the voltage to get it below 3.3v
  return result;
}
