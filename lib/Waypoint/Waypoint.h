#ifndef Waypoint_h
#define Waypoint_h

#include "Position.h"

using namespace Position;

typedef struct {
   char label[10];
   position pos;
} waypoint;

typedef struct {
   char label[9];
   int length;
   waypoint *waypoints;
} route;

inline bool operator==(const waypoint& lhs, const waypoint& rhs) {
    return (
      lhs.pos.latitude == rhs.pos.latitude &&
      lhs.pos.longitude == rhs.pos.longitude &&
      lhs.pos.error == rhs.pos.error);
}

#endif
