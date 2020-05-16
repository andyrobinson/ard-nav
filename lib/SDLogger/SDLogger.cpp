#include "Arduino.h"
#include "SDLogger.h"
#include "SPI.h"
#include "SD.h"

const int chipSelect = 4;
char logfile[13] = "nodate.csv";

calculate_filename(char *filename) {
  
}

Logger::Logger() {}

Logger::Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void Logger::begin() {
  if (!SD.begin(chipSelect)) {
    // need to do something else?
    Serial.println("Card failed, or not present");
  }
  destination = ' ';
  tack = '0';
}

void Logger::setdest(char destletter) {
  destination = destletter;
}

void Logger::settack(char tackletter) {
  tack = tackletter;
}

void Logger::banner(char *message) {
  File dataFile = SD.open(logfile, FILE_WRITE);

  if (dataFile) {
    dataFile.print("****,");
    dataFile.print("message);
    dataFile.print(",****");
    dataFile.close();
  }
}

void Logger::msg(char *message) {
  File dataFile = SD.open(logfile, FILE_WRITE);
  if (dataFile) {
    angle wind = windsensor->relative();
    uangle bearing = compass->bearing();
    gps->data(GPS_WAIT_MILLIS, &gpsReading);

    dataFile.print(gpsReading.unixTime); dataFile.print(",");
    dataFile.print(gpsReading.pos.latitude,5); dataFile.print(",");
    dataFile.print(gpsReading.pos.longitude,5); dataFile.print(",");
    dataFile.print(gpsReading.pos.error); dataFile.print(",");
    dataFile.print(gpsReading.fix); dataFile.print(",");
    dataFile.print(gpsReading.mps); dataFile.print(",");
    dataFile.print(wind); dataFile.print(",");
    dataFile.print(bearing); dataFile.print(",");
    dataFile.print(destination); dataFile.print(",");
    dataFile.print(tack); dataFile.print(",");
    dataFile.println(message);
    dataFile.close();
  }
}
