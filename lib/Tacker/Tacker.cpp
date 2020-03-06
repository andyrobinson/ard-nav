#include "Tacker.h"
#include "Angle.h"
#include "Utility.h"
#include "math.h"

using namespace Angle;
using namespace Utility;

Tacker::Tacker() {}

Tacker::Tacker(Helm *helmp, Compass *compassp, WindSensor *windsensorp):
  helm(helmp), compass(compassp), windsensor(windsensorp) {}

void Tacker::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {

  angle wind_diff = udiff(windsensor->absolute(compass->bearing()), direction);

  if (abs1(wind_diff) >= TACKER_NO_GO_LIMIT) {
    // no tack
    helm->steer(direction, steer_time, steer_interval);
  }
  else {
    short offset = TACKER_NO_GO_LIMIT - abs1(wind_diff);

    // first tack
    uangle tack_direction = uadd(direction, sign(wind_diff) * offset);
    unsigned long tack_time = round(steer_time * cos(to_radians((double) offset)));
    helm->steer(tack_direction, tack_time, steer_interval);

    // second tack
    tack_direction = uadd(tack_direction, - sign(wind_diff) * TACKER_NO_GO_LIMIT * 2);
    tack_time = round(steer_time * sin(to_radians((double) offset)));
    helm->steer(tack_direction, tack_time, steer_interval);
  }
}
