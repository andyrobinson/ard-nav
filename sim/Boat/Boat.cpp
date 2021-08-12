#include "Boat.h"

Boat::Boat(position *start, Logger *loggerp):
  logger(loggerp){
    current_position = {start->latitude, start->longitude, start->error};
  }

position Boat::location() {
  return current_position;
}
