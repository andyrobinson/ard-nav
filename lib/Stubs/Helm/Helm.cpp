#include "Helm.h"

using namespace Angle;
using namespace Utility;
using namespace Windrange;

Helm::Helm():i(0) {}

void Helm::steer(uangle direction, long steer_time, windrange range) {
  requested_direction[i] = direction;
  requested_steer_time[i] = steer_time;
  requested_windrange[i] = range;
  if (i < 100) i++;
}

int Helm::steering_calls() { return i;}

uangle Helm::steering(int index) { return requested_direction[index]; }
long Helm::steer_time(int index) {return requested_steer_time[index]; }
windrange Helm::wind_range(int index) {return requested_windrange[index];}

void Helm::reset() { i = 0; }
