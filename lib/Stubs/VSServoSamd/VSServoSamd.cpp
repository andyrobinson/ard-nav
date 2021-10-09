#include "VSServoSamd.h"

VSServoSamd::VSServoSamd() {};

void VSServoSamd::write(int value) {
  last_called = value;
};

void VSServoSamd::write(int value, uint8_t speed) {
  last_called = value;
}

int VSServoSamd::write_last_called() {
  return last_called;
};
