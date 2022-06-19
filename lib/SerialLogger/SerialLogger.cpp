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
  Serial.println("gps");  Serial.flush();
  gps->data(GPS_WAIT_MILLIS, &gpsReading);
  Serial.println("wind");  Serial.flush();
  angle wind = windsensor->relative();
  Serial.println("wind error");  Serial.flush();
  int winderr = windsensor->err_percent();
  Serial.println("compass");  Serial.flush();
  uangle bearing = compass->bearing();
  Serial.println("compass error");  Serial.flush();
  int compasserr = compass->err_percent();
  Serial.println("compass resets");  Serial.flush();
  long compass_resets = compass->resets_per_hour();
  Serial.println("memory");  Serial.flush();
  int mem=dispFreeMemory();
  Serial.println("battery");  Serial.flush();
  float voltage = battery->lipo1maxv();
  Serial.println("main log line");  Serial.flush();

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
