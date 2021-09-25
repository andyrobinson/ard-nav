#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"
#include "Angle.h"
#include "Globe.h"

#define STARTING_SPEED 1.0
#define STARTING_WIND 180
#define STARTING_HEADING 0

using namespace Position;
using namespace Angle;

class Boat
{
  public:
    Boat(position *start, Logger *loggerp);
    position location();
    double speed();
    angle relative_wind();
    uangle bearing();
    void move(unsigned long milliseconds);
    uangle heading;
    angle rudder;

  private:
    uangle new_heading(unsigned long milliseconds);
    Logger *logger;
    position current_position;
    double speed_ms;
    uangle absolute_wind;
    Globe globe;
};

#endif
