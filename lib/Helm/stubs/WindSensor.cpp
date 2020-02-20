#include "WindSensor.h"

WindSensor::WindSensor() {}

angle WindSensor::relative() {
  return current_angle;
}

void WindSensor::set_relative(angle new_angle) {
  current_angle = new_angle;
}
