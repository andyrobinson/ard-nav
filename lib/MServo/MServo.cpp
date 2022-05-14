#include "MServo.h"

MServo::MServo() {};

MServo::MServo(MicroMaestro *maestrop):
  maestro(maestrop) {}

void MServo::write(uint8_t channel, uint16_t angle) {
  angle = constrain(angle, 0, 180);

  uint16_t pulse_width = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  maestro->setTarget(channel, pulse_width);
}

void MServo::setSpeed(uint8_t channel, uint16_t speed) {
  maestro->setSpeed(channel, speed);
}

void MServo::setAccel(uint8_t channel, uint16_t accel) {
  maestro->setAcceleration(channel, accel);
}
