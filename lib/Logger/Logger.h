#ifndef Globe_h
#define Globe_h

#include "Angle.h"
#include "Position.h"

#define EARTH_RADIUS_METRES 6371000.0
#define DEGREES_IN_RADIAN 57.2958

using namespace Angle;
using namespace Position;

class Globe
{
  public:
    Globe();
    double distance_between(position *start, position *finish);
    uangle bearing(position *start, position *finish);
    position new_position(position *start, uangle bearing, double distance);

  private:
    double to_radians(double degrees);
    double to_degrees(double radians);

};

#endif
