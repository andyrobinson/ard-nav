#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), old_heading(0) {}

void Helm::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
    unsigned long elapsed = 0;
    while (elapsed < steer_time) {

      angle new_heading = compass->bearing();
      angle new_rudder_position = udiff(direction, new_heading)/2;

      if (!turning(direction, old_heading, new_heading)) {
        new_rudder_position = rudder_position + new_rudder_position;
      }

      if (abs1(new_rudder_position) > RUDDER_MAX_DISPLACEMENT) {
        new_rudder_position = sign(new_rudder_position) * RUDDER_MAX_DISPLACEMENT;
      }

      rudder->set_position(new_rudder_position);
      rudder_position = new_rudder_position;
      old_heading = new_heading;

      timer->wait(steer_interval);
      elapsed = elapsed + steer_interval;
    }
}

bool Helm::turning(uangle direction, uangle old_heading, uangle new_heading) {
    angle old_diff = udiff(direction, old_heading);
    angle new_diff = udiff(direction, new_heading);
    return abs1(new_diff) < abs1(old_diff) || sign(new_diff) != sign(old_diff);
}
