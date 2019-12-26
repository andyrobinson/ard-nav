#ifndef Globe_h
#define Globe_h

#include "Angle.h"
#include "Position.h"

#define EARTH_RADIUS_METRES 6371000.0

using namespace Angle;
using namespace Position;

class Globe
{
  public:
    Globe();
    double distance_between(Position *start, Position *finish);
    Angle bearing(Position *start, Position *finish);
    Position new_position(Position *start, angle bearing, double distance);

  private:
    double to_radians(Angle angle);

};

#endif
