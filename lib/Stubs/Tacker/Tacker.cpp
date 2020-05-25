#include "Tacker.h"

using namespace Angle;

Tacker::Tacker():i(0) {}

void Tacker::steer(uangle direction, long steer_time) {
  requested_direction[i] = direction;
  requested_steer_time[i] = steer_time;
  if (i < 100) i++;
}

uangle Tacker::steering(int index) { return requested_direction[index]; }
long Tacker::steer_time(int index) {return requested_steer_time[index]; }
bool Tacker::never_called() {return i == 0; }


void Tacker::reset() { i = 0; }
