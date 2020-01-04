#include "Servo.h"

Servo::Servo() {};

void Servo::write(int value) {
  last_called = value;
};

int Servo::write_last_called() {
  return last_called;
};
