#include "Arduino.h"
#include "SerialLogger.h"

Logger::Logger() {}

Logger::Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void Logger::begin() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
}

void Logger::msg(char *message) {
  angle wind = windsensor->relative();
  uangle bearing = compass->bearing();
  gps->data(GPS_WAIT_MILLIS, &gpsReading);

  Serial.print(gpsReading.unixTime); Serial.print(",");
  Serial.print(gpsReading.pos.latitude,5); Serial.print(",");
  Serial.print(gpsReading.pos.longitude,5); Serial.print(",");
  Serial.print("err ");Serial.print(gpsReading.pos.error); Serial.print(",");
  Serial.print("fix ");Serial.print(gpsReading.fix); Serial.print(",");
  Serial.print("m/s ");Serial.print(gpsReading.mps); Serial.print(",");
  Serial.print(wind); Serial.print(",");
  Serial.print(bearing); Serial.print(",");
  Serial.print(message);
  Serial.println();
}
