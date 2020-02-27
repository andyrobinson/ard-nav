#include "Tacker.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Tacker::Tacker() {}

Tacker::Tacker(Helm *helmp, Compass *compassp, Timer *timerp, WindSensor *windsensorp):
  helm(helmp), compass(compassp), timer(timerp), windsensor(windsensorp) {}

void Tacker::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
  // if we're not in the no go zone just steer
  // else
    // choose to tack closest to the desired course
    // tack one way
    // then tack the other

}
