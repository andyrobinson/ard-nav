#include "Helm.h"

using namespace Angle;
using namespace Utility;

Helm::Helm() {}

void Helm::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
  requested_direction = direction;
}

uangle Helm::steering() {
  return requested_direction;
}
