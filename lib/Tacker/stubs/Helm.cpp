#include "Helm.h"

using namespace Angle;
using namespace Utility;

Helm::Helm() {}

void Helm::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
  requested_direction = direction;
  requested_steer_time = steer_time;
  requested_interval = steer_interval;
}

uangle Helm::steering() { return requested_direction; }

long Helm::steer_time() {return requested_steer_time; }
long Helm::interval() {return requested_interval; }
