#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"
#include "Angle.h"
#include "Globe.h"
#include "Utility.h"

#define STARTING_SPEED 1.0
#define STARTING_WIND 237
#define STARTING_HEADING 0
#define HULL_SPEED_MS 1.2
#define HULL_DRAG_CONSTANT 2.6

using namespace Position;
using namespace Angle;
using namespace Utility;

class Boat
{
  public:
    Boat(position *start);
    position location();
    double speed();
    angle relative_wind();
    uangle bearing();
    void move(unsigned long milliseconds);
    void setLogger(Logger *loggerp);
    double drag(double speed);
    uangle heading;
    uint16_t rudder;
    uint16_t sail;

  private:
    uangle new_heading(unsigned long milliseconds);
    Logger *logger;
    position current_position;
    double speed_ms;
    uangle absolute_wind;
    Globe globe;
};

#endif
