#include "Arduino.h"
#include "SerialLogger.h"

SerialLogger::SerialLogger() {}

SerialLogger::SerialLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp):
  gps(gpsp), compass(compassp), windsensor(windsensorp), battery(batteryp) {}

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
  gps->data(GPS_WAIT_MILLIS, &gpsReading);
  angle wind = windsensor->relative();
  int winderr = windsensor->err_percent();
  uangle bearing = compass->bearing();
  int compasserr = compass->err_percent();
  long compass_resets = compass->resets_per_hour();
  int mem=dispFreeMemory();
  float voltage = battery->lipo1maxv();  // CRASHES HERE

  Serial.print(gpsReading.unixTime); Serial.print(",");
  Serial.print(millis()/1000); Serial.print(",");
  Serial.print(gpsReading.pos.latitude,5); Serial.print(",");
  Serial.print(gpsReading.pos.longitude,5); Serial.print(",");
  Serial.print("err ");Serial.print(gpsReading.pos.error); Serial.print(",");
  Serial.print("fix ");Serial.print(gpsReading.fix); Serial.print(",");
  Serial.print("m/s ");Serial.print(gpsReading.mps); Serial.print(",");
  Serial.print(voltage,2); Serial.print(",");
  Serial.print(mem); Serial.print(",");
  Serial.print(wind); Serial.print(",");
  Serial.print(winderr); Serial.print(",");
  Serial.print(bearing); Serial.print(",");
  Serial.print(compasserr); Serial.print(",");
  Serial.print(compass_resets); Serial.print(",");
  Serial.print(destination); Serial.print(",");
  Serial.print(tack); Serial.print(",");
  Serial.print(message);
  Serial.println();
}
