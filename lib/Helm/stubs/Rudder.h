#ifndef RudderStub_h
#define RudderStub_h

#include "Angle.h"

#define RUDDER_MAX_DISPLACEMENT 45

using namespace Angle;

class Rudder
{
  public:
    Rudder();
    void set_position(angle);
    angle get_position();
};

#endif
