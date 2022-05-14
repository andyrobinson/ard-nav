#include "Arduino.h"
#include "SDLogger.h"
#include "Utility.h"
#include "SPI.h"
#include "SD.h"

using namespace Utility;

SDLogger::SDLogger() {}

SDLogger::SDLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp):
  gps(gpsp), compass(compassp), windsensor(windsensorp), battery(batteryp) {}

void SDLogger::calculate_filename(char *filename, long unix_ts) {
    long filenameint = 2025; //max1(unix_ts / 100000, JAN1_2000_TS);
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
  banner_msg[0] = '\0';
  banner_space = LOG_BANNER_LENGTH-1;
}

void SDLogger::setdest(char destletter) {
  destination = destletter;
}

void SDLogger::settack(char tackletter) {
  tack = tackletter;
}

void SDLogger::banner(char *message) {
    // keep hold of banner messages until we can print
    if (banner_space > strlen(message)) {
      strcat(banner_msg, message);
      strcat(banner_msg, " ");
      banner_space = banner_space - (strlen(message) + 1);
    }
    banner_msg[LOG_BANNER_LENGTH-1]='\0'; // belt and braces
    msg("");
}

void SDLogger::print_line(char *message) {
    gps->data(GPS_WAIT_MILLIS, &gpsReading);
    angle wind = windsensor->relative();
    int winderr = windsensor->err_percent();
    uangle bearing = compass->bearing();
    int compasserr = compass->err_percent();
    long compass_resets = compass->resets_per_hour();
    int mem=dispFreeMemory();
    float voltage = battery->lipo1maxv();

    calculate_filename(logfile, gpsReading.unixTime);
    File dataFile = SD.open(logfile, FILE_WRITE);

    if (dataFile) {

      dataFile.print(gpsReading.unixTime); dataFile.print(",");
      dataFile.print(millis()/1000); dataFile.print(",");
      dataFile.print(gpsReading.pos.latitude,5); dataFile.print(",");
      dataFile.print(gpsReading.pos.longitude,5); dataFile.print(",");
      dataFile.print(gpsReading.pos.error); dataFile.print(",");
      dataFile.print(gpsReading.fix); dataFile.print(",");
      dataFile.print(gpsReading.mps); dataFile.print(",");
      dataFile.print(voltage,2); dataFile.print(",");
      dataFile.print(mem); dataFile.print(",");
      dataFile.print(wind); dataFile.print(",");
      dataFile.print(winderr); dataFile.print(",");
      dataFile.print(bearing); dataFile.print(",");
      dataFile.print(compasserr); dataFile.print(",");
      dataFile.print(compass_resets); dataFile.print(",");
      dataFile.print(destination); dataFile.print(",");
      dataFile.print(tack); dataFile.print(",");
      dataFile.print(message);
      if (banner_msg[0] != '\0') {
        dataFile.print(" **** "), dataFile.print(banner_msg);
        banner_msg[0]='\0';
        banner_space = LOG_BANNER_LENGTH-1;
      }
      dataFile.println("");
      dataFile.close();
    }
}

void SDLogger::msg(char *message) {
  if (sd_time_to_log()) {
    print_line(message);
  }
}
