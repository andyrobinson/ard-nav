#ifndef Route_h
#define Route_h

#include "Waypoint.h"

class Route
{
  public:
    virtual void begin(route *rt, bool cold_start) = 0;
    virtual bool done() = 0;
    virtual waypoint *next() = 0;
};

#endif
