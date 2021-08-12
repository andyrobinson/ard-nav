#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"

using namespace Position;

class Boat
{
  public:
    Boat(position *start, Logger *loggerp);
    position location();

  private:
    Logger *logger;
    position current_position;
};

#endif
