#include "Switches.h"

Switches::Switches() {}

void Switches::set(uint8_t value) {
  input_value = value;
}

uint8_t Switches::value() {
  return input_value;
}

void Switches::set_percent(float value) {
    percent = value;
}

float Switches::dial_percent() {
    return percent;
}

