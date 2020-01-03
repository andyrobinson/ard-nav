#include "Helm.h"
#include "Angle.h"

using namespace Angle;

Helm::Helm() {
  rudder_position = 0;
}

Helm::Helm(Rudder *rudderp, Compass *compassp) {
  rudder = *rudderp;
  compass = *compassp;
}

void Helm::steer(uangle direction) {
    angle delta = udiff(direction, compass.bearing());
    rudder.set_position(delta/2);
}
