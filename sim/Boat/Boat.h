#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"
#include "Timer.h"

using namespace Position;

class Boat
{
  public:
    Boat(Timer *timerp, Logger *loggerp);
    position location();

  private:
    Logger *logger;
    Timer *timer;
};

#endif
