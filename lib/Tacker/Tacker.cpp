#include "Tacker.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Tacker::Tacker() {}

Tacker::Tacker(Helm *helmp, Compass *compassp, WindSensor *windsensorp):
  helm(helmp), compass(compassp), windsensor(windsensorp) {}

void Tacker::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {

  helm->steer(direction, steer_time, steer_interval);
  // if we're not in the no go zone just steer
  // else
    // choose to tack closest to the desired course
    // divide the time up between the two tacks
    // tack one way
    // then tack the other

}
