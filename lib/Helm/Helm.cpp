#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp) {}

void Helm::steer(uangle direction, unsigned long millis, unsigned long waittime) {
    unsigned long elapsed = 0;
    while (elapsed < millis) {
      angle new_position = udiff(direction, compass->bearing())/2;
      if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
        new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
      }
      rudder->set_position(new_position);
      timer->wait(waittime);
      elapsed = elapsed + waittime;
    }
}
