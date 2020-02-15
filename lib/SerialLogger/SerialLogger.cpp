#include "Arduino.h"
#include "SerialLogger.h"

Logger::Logger() {}

void Logger::begin() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
}

void Logger::info(gpsResult *gps_result, angle wind, uangle bearing, String message) {
  Serial.print(gps_result->unixTime); Serial.print(",");
  Serial.print(gps_result->pos.latitude,5); Serial.print(",");
  Serial.print(gps_result->pos.longitude,5); Serial.print(",");
  Serial.print("err ");Serial.print(gps_result->pos.error); Serial.print(",");
  Serial.print(gps_result->fix); Serial.print(",");
  Serial.print("kn ");Serial.print(gps_result->knots); Serial.print(",");
  Serial.print(wind); Serial.print(",");
  Serial.print(bearing); Serial.print(",");
  Serial.print(message);
  Serial.println();
}
