#include <iostream>
#include "ConsoleLogger.h"

ConsoleLogger::ConsoleLogger() {}

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
}
