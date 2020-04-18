#ifndef NavigatorStub_h
#define NavigatorStub_h

#include "Waypoint.h"

class Navigator
{
  public:
    Navigator();
    void sailto(waypoint destination);
    waypoint *get_waypoints();
    void reset();

  private:
    waypoint waypoints[100];
    int current_position;
};

#endif
