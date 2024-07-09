#ifndef Boat_h
#define Boat_h

#include <stdint.h>
#include "Logger.h"
#include "Position.h"
#include "Angle.h"
#include "Globe.h"
#include "Utility.h"
#include "math.h"
#include "LiftDrag.h"

#define STARTING_SPEED 0.0
#define STARTING_WIND_DIRECTION 237
#define STARTING_WIND_SPEED 10.0
#define STARTING_HEADING 0
#define STARTING_SAIL 45

#define HULL_SPEED_MS 1.2
#define HULL_DRAG_CONSTANT 2.6
#define BOAT_MASS_KG 13.4
#define TIME_INCREMENT 100
#define SAIL_COE_TO_COG 0.71
#define METACENTRE 0.1 // compromise, actually alters with heel

using namespace Position;
using namespace Angle;
using namespace Utility;
using namespace LiftDrag;

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
    double hull_drag(double speed);
    double sail_force();
    double new_speed(double speed, double force, double drag, long millis);
    double heel(double lateral_force);
    uangle heading;
    uangle absolute_wind;
    uint16_t rudder;
    uint16_t sail;
    double wind_speed;
    double heel_angle;

  private:
    uangle new_heading(unsigned long milliseconds);
    Logger *logger;
    position current_position;
    double speed_ms;
    Globe globe;
};

#endif
