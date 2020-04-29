#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, Logger *loggerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), windsensor(windsensorp), sail(sailp), logger(loggerp), old_heading(0) {}

void Helm::steer(uangle direction, unsigned long steer_time, unsigned long steer_interval) {
    unsigned long elapsed = 0;

    char logmsg[40];

    while (elapsed < steer_time) {

      angle current_heading = compass->bearing();
      angle new_rudder_position = new_rudder(direction, current_heading);

      set_rudder(new_rudder_position, current_heading);
      sail->set_position(windsensor->relative());

      timer->wait(steer_interval);
      elapsed = elapsed + steer_interval;

      sprintf(logmsg, "Steer %4d %8d", direction, elapsed); logger->msg(logmsg);
    }
}

void Helm::set_rudder(angle new_position, uangle current_heading) {
  rudder->set_position(new_position);
  rudder_position = new_position;
  old_heading = current_heading;
}

/*
New algorithm
  rot = rate of turn in degrees/sec, using the steer_interval (max 180)

  if (rot big enough [> 2] and in desired direction) {
     if (rot too big (more than three times remaining angle)) {
	[ implies that we don't reduce rudder until we're within 60 degrees ]
       negative nudge [fixed amount, say 5 degrees, ok to go negative]
     }
     do nothing
  } else {
    positive nudge [fixed amount]
  }


*/

angle Helm::new_rudder(uangle direction, uangle current_heading) {
  angle new_position = udiff(direction, current_heading)/2;

  if (!turning(direction, old_heading, current_heading)) {
    new_position = rudder_position + 2;
  }

  if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
    new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
  }

  return new_position;
}

bool Helm::turning(uangle direction, uangle old_heading, uangle new_heading) {
    angle old_diff = udiff(direction, old_heading);
    angle new_diff = udiff(direction, new_heading);
    angle diff_diff = abs1(udiff(old_diff, new_diff));
    return ((abs1(new_diff) < abs1(old_diff)) && diff_diff > TURNING_MIN_DIFF) || sign(new_diff) != sign(old_diff);
}
