#include "Arduino.h"
#include "MultiLogger.h"

MultiLogger::MultiLogger() {}

MultiLogger::MultiLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger **loggersp, int num):
  gps(gpsp), compass(compassp), windsensor(windsensorp), loggers(loggersp), numLoggers(num) {}

void MultiLogger::begin() {
  for (int i; i < numLoggers; i++) {
    loggers[i]->begin();
  }
}

void MultiLogger::setdest(char destletter) {
  for (int i; i < numLoggers; i++) {
    loggers[i]->setdest(destletter);
  }
}

void MultiLogger::settack(char tackletter) {
  for (int i; i < numLoggers; i++) {
    loggers[i]->settack(tackletter);
  }
}

void MultiLogger::banner(char *message) {
  for (int i; i < numLoggers; i++) {
    loggers[i]->banner(message);
  }
}

void MultiLogger::msg(char *message) {
  for (int i; i < numLoggers; i++) {
    loggers[i]->msg(message);
  }
}
