#include "Switches.h"

Switches::Switches() {}

void Switches::begin() {
  pinMode(SWITCH1, INPUT);
  pinMode(SWITCH2, INPUT);
  pinMode(SWITCH4, INPUT);
}

uint8_t Switches::value() {
  uint8_t result = (4 * digitalRead(SWITCH4)) + (2 * digitalRead(SWITCH2)) + digitalRead(SWITCH1);
  return result;
}