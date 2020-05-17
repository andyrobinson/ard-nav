#include "Arduino.h"
#include "MultiLogger.h"

Logger::Logger() {}

Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger *loggersp, int num):
  gps(gpsp), compass(compassp), windsensor(windsensorp), loggers(loggersp), numLoggers(num) {}

void Logger::begin() {
  for (int i; i < numLoggers; i++) {
    loggers[i].begin();
  }
}

void Logger::setdest(char destletter) {
  for (int i; i < numLoggers; i++) {
    loggers[i].setdest(destletter);
  }
}

void Logger::settack(char tackletter) {
  for (int i; i < numLoggers; i++) {
    loggers[i].settack(tackletter);
  }
}

void Logger::banner(char *message) {
  for (int i; i < numLoggers; i++) {
    loggers[i].banner(char);
  }
}

void Logger::msg(char *message) {
  for (int i; i < numLoggers; i++) {
    loggers[i].msg(message);
  }
}
