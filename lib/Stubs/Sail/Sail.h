#ifndef SailStub_h
#define SailStub_h

#include "Angle.h"

using namespace Angle;

class Sail
{
  public:
    Sail();
    void set_position(angle relative_wind);
    angle* get_calls();
    void reset();

  private:
    angle sail_calls[100];
    int current_position;

};

#endif
