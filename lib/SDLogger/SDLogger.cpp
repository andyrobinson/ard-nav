#include "Arduino.h"
#include "SDLogger.h"
#include "Utility.h"
#include "SPI.h"
#include "SD.h"

using namespace Utility;

SDLogger::SDLogger() {}

SDLogger::SDLogger(Compass *compassp):
  compass(compassp) {}

void SDLogger::calculate_filename(char *filename, long unix_ts) {
<<<<<<< HEAD
    long filenameint = 2022; // max1(unix_ts / 100000, JAN1_2000_TS);
=======
    long filenameint = TEST_LOG_FILE;
>>>>>>> parent of 7e1a0db... UNKNOWN: nav.ino with new libs
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
<<<<<<< HEAD
  if (!SD.begin(CHIP_SELECT)) {
    // card failure, do nothing
  }
=======
  SD.begin(CHIP_SELECT); // ignore return, we can't do anything with it
>>>>>>> parent of 7e1a0db... UNKNOWN: nav.ino with new libs
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
    calculate_filename(logfile, 10l);
    File dataFile = SD.open(logfile, FILE_WRITE);
    if (dataFile) {
      uangle bearing = compass->bearing();
      int compass_err = compass->err_percent();
      int mem=dispFreeMemory();

      dataFile.print(millis()/1000); dataFile.print(",");
      dataFile.print(mem); dataFile.print(",");
      dataFile.print(bearing); dataFile.print(",");
      dataFile.print(compasserr); dataFile.print(",[");
      dataFile.print(tol); dataFile.print("],");
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
