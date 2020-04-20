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
    sprintf(logmsg, "Steer %4d fr %8d", direction, steer_time);
    logger->msg(logmsg);

    while (elapsed < steer_time) {

      angle current_heading = compass->bearing();
      angle new_rudder_position = new_rudder(direction, current_heading);

      set_rudder(new_rudder_position, current_heading);
      sail->set_position(windsensor->relative());

      timer->wait(steer_interval);
      elapsed = elapsed + steer_interval;

      // char nothing = '-';
      // logger->msg(&nothing);
    }
}

void Helm::set_rudder(angle new_position, uangle current_heading) {
  rudder->set_position(new_position);
  rudder_position = new_position;
  old_heading = current_heading;
}

angle Helm::new_rudder(uangle direction, uangle current_heading) {
  angle new_position = udiff(direction, current_heading)/2;

  if (!turning(direction, old_heading, current_heading)) {
    new_position = rudder_position + new_position;
  }

  if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
    new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
  }

  return new_position;
}

bool Helm::turning(uangle direction, uangle old_heading, uangle new_heading) {
    angle old_diff = udiff(direction, old_heading);
    angle new_diff = udiff(direction, new_heading);
    return abs1(new_diff) < abs1(old_diff) || sign(new_diff) != sign(old_diff);
}
