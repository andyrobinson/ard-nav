#include "Tacker.h"

using namespace Angle;

Tacker::Tacker():i(0) {}

void Tacker::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
  requested_direction[i] = direction;
  requested_steer_time[i] = steer_time;
  requested_interval[i] = steer_interval;
  if (i < 100) i++;
}

uangle Tacker::steering(int index) { return requested_direction[index]; }
long Tacker::steer_time(int index) {return requested_steer_time[index]; }
long Tacker::interval(int index) {return requested_interval[index]; }
bool Tacker::never_called() {return i == 0; }


void Tacker::reset() { i = 0; }
