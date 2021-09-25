#ifndef RudderSim_h
#define RudderSim_h

#include "Angle.h"
#include "Boat.h"

#define RUDDER_MAX_DISPLACEMENT 45

using namespace Angle;

class Rudder
{
  public:
    Rudder();
    Rudder(Boat *boatp);
    void set_position(angle);

  private:
    Boat *boat;

};

#endif
