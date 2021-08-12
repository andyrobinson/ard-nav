#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"

using namespace Position;

class Boat
{
  public:
    Boat(Logger *loggerp);
    position location();

  private:
    Logger *logger;
};

#endif
