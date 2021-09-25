#ifndef SailSim_h
#define SailSim_h

#include "Angle.h"
#include "Boat.h"

#define ANGLE_OF_ATTACK        30
#define PURE_LIFT_LIMIT        90
#define LIFT_TO_DRAG_LIMIT     140
#define GYBE_CHECK_LIMIT       80
#define GYBE_CHECK_MAX_DIFF    10
#define SERVO_MAX_DISPLACEMENT 90

using namespace Angle;

class Sail
{
  public:
    Sail();
    Sail(Boat *boatp);
    void set_position(angle relative_wind);

  private:
    angle sail_position(angle relative_wind);
    angle gybe_check(angle old_position, angle new_position);
    Boat *boat;

};

#endif
