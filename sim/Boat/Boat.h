#ifndef Boat_h
#define Boat_h

#include "Logger.h"
#include "Position.h"
#include "Angle.h"
#include "Globe.h"
#include "Utility.h"
#include "Math.h"

#define STARTING_SPEED 0.0
#define STARTING_WIND 237
#define STARTING_HEADING 0
#define HULL_SPEED_MS 1.2
#define HULL_DRAG_CONSTANT 2.6
#define BOAT_MASS_KG 13.4
#define TIME_INCREMENT 100

using namespace Position;
using namespace Angle;
using namespace Utility;

class Boat
{
  public:
    Boat(position *start, double starting_speed = STARTING_SPEED);
    position location();
    double speed();
    angle relative_wind();
    uangle bearing();
    void move(unsigned long milliseconds);
    void setLogger(Logger *loggerp);
    double drag(double speed);
    double sail_force();
    double new_speed(double speed, double force, double drag, long millis);
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
