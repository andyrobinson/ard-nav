#include "Switches.h"

Switches::Switches() {}

void Switches::set(uint8_t value) {
  input_value = value;
}

uint8_t Switches::value() {
  return input_value;
}
