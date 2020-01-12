#include "Arduino.h"
#include "Logger.h"

Logger::Logger() {}

void Logger::begin() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
}

void Logger::info(position *current_position, angle wind, uangle bearing, String message) {
  Serial.print(current_position->latitude,5); Serial.print(",");
  Serial.print(current_position->longitude,5); Serial.print(",");
  Serial.print(angle); Serial.print(",");
  Serial.print(bearing); Serial.print(",");
  Serial.print(message);
  Serial.println();
}
