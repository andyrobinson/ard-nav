#ifndef Captain_h
#define Captain_h

#include "Waypoint.h"
#include "Navigator.h"

using namespace Position;

class Captain
{
  public:
    Captain();
    Captain(Navigator *navigatorp);
    void voyage(waypoint *route, int length);

  private:
    Navigator *navigator;
};

#endif
