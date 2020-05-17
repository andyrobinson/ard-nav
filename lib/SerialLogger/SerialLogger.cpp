#include "Arduino.h"
#include "SerialLogger.h"

SerialLogger::SerialLogger() {}

SerialLogger::SerialLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger *loggersp, int num):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void SerialLogger::begin() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  destination = ' ';
  tack = '0';
}

void SerialLogger::setdest(char destletter) {
  destination = destletter;
}

void SerialLogger::settack(char tackletter) {
  tack = tackletter;
}

void SerialLogger::banner(char *message) {
  Serial.println("======================");
  Serial.println(message);
  Serial.println("======================");
}

void SerialLogger::msg(char *message) {
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
  Serial.print(destination); Serial.print(",");
  Serial.print(tack); Serial.print(",");
  Serial.print(message);
  Serial.println();
}
