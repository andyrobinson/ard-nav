#ifndef CompassStub_h
#define CompassStub_h

#include "Angle.h"

using namespace Angle;

class Compass
{
  public:
    Compass();
    uangle bearing();
    int err_percent();
    long resets_per_hour();

    void set_bearings(uangle *bearings, int size);
    void set_err_percent(int err_percent);

  private:
    int err;
    int list_size;
    int current_position;
    uangle *response_list;
};

#endif
