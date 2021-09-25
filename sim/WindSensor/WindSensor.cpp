#include "WindSensor.h"

WindSensor::WindSensor(Boat *boatp):boat(boatp) {}

angle WindSensor::relative() {
    return boat->relative_wind();
}

uangle WindSensor::absolute(uangle bearing) {
  return uadd(bearing, relative());
}
