#include "Arduino.h"
#include "SDLogger.h"
#include "Utility.h"
#include "SPI.h"
#include "SD.h"

using namespace Utility;

SDLogger::SDLogger() {}

SDLogger::SDLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void SDLogger::calculate_filename(char *filename, long unix_ts) {
    long filenameint = 2022; //max1(unix_ts / 100000, JAN1_2000_TS);
    itoa(filenameint, filename, BASE10);
    strcat(filename,".csv");
}

boolean SDLogger::sd_time_to_log() {
  boolean its_time = (millis() - sd_last_log_time) > LOG_INTERVAL;
  if (its_time) {
    sd_last_log_time = millis();
  }
  return its_time;
}

void SDLogger::begin() {
  if (!SD.begin(CHIP_SELECT)) {
    // need to do something else?
    Serial.println("Card failed, or not present");
  }
  logfile[0] = '\0';
  unsigned long sd_last_log_time = 0;
  destination = ' ';
  tack = '0';
}

void SDLogger::setdest(char destletter) {
  destination = destletter;
}

void SDLogger::settack(char tackletter) {
  tack = tackletter;
}

void SDLogger::banner(char *message) {
    print_line(message, "*** ");
}

void SDLogger::print_line(char *message, char *msgprefix) {
    gps->data(GPS_WAIT_MILLIS, &gpsReading);
    calculate_filename(logfile, gpsReading.unixTime);
    File dataFile = SD.open(logfile, FILE_WRITE);
    if (dataFile) {
      angle wind = windsensor->relative();
      uangle bearing = compass->bearing();
      int mem=dispFreeMemory();

      dataFile.print(gpsReading.unixTime); dataFile.print(",");
      dataFile.print(millis()/1000); dataFile.print(",");
      dataFile.print(gpsReading.pos.latitude,5); dataFile.print(",");
      dataFile.print(gpsReading.pos.longitude,5); dataFile.print(",");
      dataFile.print(gpsReading.pos.error); dataFile.print(",");
      dataFile.print(gpsReading.fix); dataFile.print(",");
      dataFile.print(gpsReading.mps); dataFile.print(",");
      dataFile.print(mem); dataFile.print(",");
      dataFile.print(wind); dataFile.print(",");
      dataFile.print(bearing); dataFile.print(",");
      dataFile.print(destination); dataFile.print(",");
      dataFile.print(tack); dataFile.print(",");
      dataFile.print(msgprefix);dataFile.println(message);
      dataFile.close();
    }
}

void SDLogger::msg(char *message) {
  if (sd_time_to_log()) {
    print_line(message, "");
  }
}
