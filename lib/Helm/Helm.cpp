#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp):
  rudder_position(0),rudder(rudderp), compass(compassp) {}

void Helm::steer(uangle direction) {
    angle new_position = udiff(direction, compass->bearing())/2;
    if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
      new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
    }
    rudder->set_position(new_position);
}
