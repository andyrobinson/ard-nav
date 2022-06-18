#include "WindSensor.h"

WindSensor::WindSensor():response_list{0},list_size(1),current_position(0) {}

angle WindSensor::relative() {
  angle result = response_list[current_position];
  if (current_position + 1 < list_size) current_position++;
  return result;
}

uangle WindSensor::absolute(uangle bearing) {
  angle rel = relative();
  return rel == ANGLE_ERROR ? ANGLE_ERROR : uadd(bearing, rel);
}

void WindSensor::set_relative(angle *rel_angles, int size) {
  response_list = rel_angles;
  list_size = size;
  current_position = 0;
}
