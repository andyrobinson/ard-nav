#include "Helm.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():i(0) {}

void Helm::steer(uangle direction, long steer_time) {
  requested_direction[i] = direction;
  requested_steer_time[i] = steer_time;
  if (i < 100) i++;
}

uangle Helm::steering(int index) { return requested_direction[index]; }
long Helm::steer_time(int index) {return requested_steer_time[index]; }

void Helm::reset() { i = 0; }
