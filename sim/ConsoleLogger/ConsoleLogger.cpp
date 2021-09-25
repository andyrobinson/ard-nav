#include <iostream>
#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger() {}
ConsoleLogger::ConsoleLogger(Boat *boatp, Timer *timerp):boat(boatp),timer(timerp){}

void ConsoleLogger::begin() {
}

void ConsoleLogger::setdest(char destletter) {
  destination = destletter;
}

void ConsoleLogger::settack(char tackletter) {
  tack = tackletter;
}

void ConsoleLogger::banner(char *message) {
  std::cout << "\n==========================================\n";
  std::cout << message;
  std::cout << "\n==========================================\n\n";
}

void ConsoleLogger::msg(char *message) {
  std::cout << message;

  angle wind = windsensor->relative();
  uangle bearing = compass->bearing();
  gps->data(GPS_WAIT_MILLIS, &gpsReading);

  std::cout << timer->elapsed(); std::cout << ",";
  std::cout << boat->location().latitude; std::cout << ",";
  std::cout << boat->location().longitude; std::cout << ",";
  std::cout << boat->speed(); std::cout << "ms,";
  std::cout << boat->relative_wind(); std::cout << ",";
  std::cout << boat->bearing(); std::cout << ",";
  std::cout << destination; std::cout << ",";
  std::cout << tack; std::cout << ",";
  std::cout << message;
  std::cout << "\n";
}
