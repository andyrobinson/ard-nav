#include "MServo.h"

MServo::MServo() {};
MServo::MServo(Boat *boatp):boat(boatp) {};

void MServo::write(uint8_t channel, uint16_t angle) {
  angle = constrain(angle, (uint16_t) 0, (uint16_t) 180);

  if (channel == SIM_RUDDER_CHANNEL) {
    boat->rudder = angle;
  } else if (channel == SIM_SAIL_CHANNEL) {
    boat->sail = angle;
  }
}

// no-op to begin with
void MServo::begin() {}
void MServo::setSpeed(uint8_t channel, uint16_t speed) {}
void MServo::setAccel(uint8_t channel, uint16_t accel) {}
