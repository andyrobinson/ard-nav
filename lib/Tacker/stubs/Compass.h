#ifndef CompassStub_h
#define CompassStub_h

#include "Angle.h"

using namespace Angle;

class Compass
{
  public:
    Compass();
    uangle bearing();
    void set_bearings(uangle *bearings, int size);
  private:
    int list_size;
    int current_position;
    uangle *response_list;
};

#endif
