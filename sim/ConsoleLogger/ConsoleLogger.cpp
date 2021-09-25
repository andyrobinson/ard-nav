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
  std::cout << timer->elapsed() << ",";
  std::cout << boat->location().latitude  << ",";
  std::cout << boat->location().longitude << ",";
  std::cout << boat->speed() << "ms,";
  std::cout << boat->relative_wind() << ",";
  std::cout << boat->bearing() << ",";
  std::cout << destination << ",";
  std::cout << tack << ",";
  std::cout << message;
  std::cout << "\n";
}
