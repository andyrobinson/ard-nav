#include "Battery.h"

Battery::Battery() {}

float Battery::lipo1v() {
  int pin_voltage = analogRead(LIPO1);
  float result = 3.3 * ((float) pin_voltage)/MAX_ANALOG;
  return result;
}
