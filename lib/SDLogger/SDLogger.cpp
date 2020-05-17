#include "Arduino.h"
#include "SDLogger.h"
#include "Utility.h"
#include "SPI.h"
#include "SD.h"

char logfile[13] = "blank000.csv";
unsigned long sd_last_log_time = 0;

calculate_filename(char *filename, long unix_ts) {
    long filenameint = max1(unix_ts / 100000, JAN1_2000_TS);
    itoa(filenameint, filename, BASE10);
    strcat(filename,".csv");
}

boolean sd_time_to_log() {
  boolean its_time = (millis() - sd_last_log_time) > LOG_INTERVAL
  if (its_time) {
    sd_last_log_time = millis();
  }
  return its_time;
}

Logger::Logger() {}

Logger::Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger *loggersp, int num):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void Logger::begin() {
  if (!SD.begin(CHIP_SELECT)) {
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
    dataFile.println(",****");
    dataFile.close();
  }
}

void Logger::msg(char *message) {
  if (sd_time_to_log()) {
    gps->data(GPS_WAIT_MILLIS, &gpsReading);
    calculate_filename(logfile, gpsReading.unixTime);
    File dataFile = SD.open(logfile, FILE_WRITE);
    if (dataFile) {
      angle wind = windsensor->relative();
      uangle bearing = compass->bearing();

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
}
