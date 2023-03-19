#include <iostream>
#include <iomanip>
#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger() {}
ConsoleLogger::ConsoleLogger(Boat *boatp, Timer *timerp):boat(boatp),timer(timerp),last_logged(0){}

void ConsoleLogger::begin() {
}

void ConsoleLogger::setdest(char destletter) {
  destination = destletter;
}

void ConsoleLogger::settack(char tackletter) {
  tack = tackletter;
}

void ConsoleLogger::banner(char *message) {
  char line[] = "==========================================";
  logmsg(line);
  logmsg(message);
  logmsg(line);
}

void ConsoleLogger::msg(char *message) {
    long time_now = timer->elapsed();
    if (last_logged + LOGGING_INTERVAL < time_now) {
        last_logged = time_now;
        logmsg(message);
    }
}

void ConsoleLogger::logmsg(char * message) {
  std::setprecision(6);
  std::cout << timer->elapsed()/1000 << ",";
  // need 5 decimal places (at least 7 digits) for metres
  std::cout << std::setprecision(8) << boat->location().latitude  << ",";
  std::cout << std::setprecision(8) << boat->location().longitude << ",";
  std::cout << std::setprecision(4) <<boat->speed() << "ms,";
  std::cout << boat->relative_wind() << "wd,";
  std::cout << boat->bearing() << "bg,";
  std::cout << boat->rudder << "rd,";
  std::cout << destination << ",";
  std::cout << tack << ",";
  std::cout << message;
  std::cout << "\n";
}

void ConsoleLogger::write_version(int major, int minor) {
  std::cout << "Version: " << major << "." << minor << "\n";
}
