#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"
#include "Angle.h"
#include "Globe.h"

using namespace Position;
using namespace Angle;

class Boat
{
  public:
    Boat(position *start, Logger *loggerp);
    position location();
    void move(unsigned long milliseconds);
    uangle heading;
    angle rudder;

  private:
    Logger *logger;
    position current_position;
    double speed_ms;
    Globe globe;
};

#endif
