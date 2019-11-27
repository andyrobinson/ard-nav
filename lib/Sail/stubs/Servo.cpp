#include "Servo.h"

Servo::Servo() {};

int last_called(-2);

void Servo::write(int value) {
  last_called = value;
};

int Servo::write_last_called() {
  return last_called;
};
