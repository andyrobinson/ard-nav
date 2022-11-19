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

// 77 to 145

float Switches::dial_percent() {
  int pin_voltage = analogRead(RCINPUT);
  float result = constrain(100.0 * ((float) pin_voltage - RCLOWER)/(RCUPPER - RCLOWER),0.0,100.0);
  return result;
}
