#ifndef NavigatorStub_h
#define NavigatorStub_h

#include "Position.h"

using namespace Position;

class Navigator
{
  public:
    Navigator();
    void sailto(position destination);
    position *get_waypoints();
    void reset();

  private:
    position positions[100];
    int current_position;
};

#endif
