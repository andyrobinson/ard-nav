#include "Boat.h"

Boat::Boat(Timer *timer, Logger *loggerp):
  logger(loggerp){}

position Boat::location() {
  return  {49.97480, -5.23198, 5.0};
}
