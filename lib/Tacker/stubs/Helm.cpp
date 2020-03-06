#include "Helm.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():i(0) {}

void Helm::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
  requested_direction[i] = direction;
  requested_steer_time[i] = steer_time;
  requested_interval[i] = steer_interval;
  if (i < 100) i++;
}

uangle Helm::steering(int index) { return requested_direction[index]; }
long Helm::steer_time(int index) {return requested_steer_time[index]; }
long Helm::interval(int index) {return requested_interval[index]; }

void Helm::reset() { i = 0; }
