#include "Helm.h"
#include "Angle.h"

using namespace Angle;

Helm::Helm() {
  rudder_position = 0;
}

Helm::Helm(Rudder *rudder, Compass *compass) {
  rudder = rudder;
  compass = compass;
}

void Helm::steer(uangle direction) {
    angle delta = udiff(compass->bearing(), direction);
    rudder->set_position(delta/2);
}
