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

    char logmsg[20];

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

angle Helm::new_rudder(uangle direction, uangle current_heading) {

  angle new_position;

  if (heading_and_turn_ok(direction, old_heading, current_heading)) {
    new_position =  rudder_position; // no change
  } else {
    long desired_rot = udiff(current_heading, direction);
    new_position = new_position - sign(desired_rot) * NUDGE_DEGREES; // increase deflection

    // long actual_rot = rot(old_heading, current_heading, steer_interval);
    //
    // if ((sign(desired_rot) != sign(desired_rot)) || (abs1(desired_rot) - abs1(actual_rot)) > MIN_DIFF_DEGREES) {
    //     new_position = new_position - sign(desired_rot) * NUDGE_DEGREES; // increase deflection
    // } else if ((abs1(desired_rot) - abs1(actual_rot)) < MIN_DIFF_DEGREES) {
    //     new_position = new_position - sign(desired_rot) * NUDGE_DEGREES; // decrease deflection
    // } else {
    //   new_position = rudder_position; // no change
    // }
  }

  if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
    new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
  }

  return new_position;
}

bool Helm::heading_and_turn_ok(uangle direction, uangle old_heading, uangle current_heading) {
  return (abs1(udiff(direction, current_heading)) <= MIN_DIFF_DEGREES) &&
  (abs1(udiff(current_heading, old_heading)) <= MIN_DIFF_DEGREES);
}

bool Helm::more_steerage(angle new_position) {
  return abs1(rudder_position) < abs1(new_position) || sign(rudder_position) != sign(new_position);
}

bool Helm::turning(uangle direction, uangle old_heading, uangle new_heading) {
    angle old_diff = udiff(direction, old_heading);
    angle new_diff = udiff(direction, new_heading);
    angle diff_diff = abs1(udiff(old_diff, new_diff));
    return ((abs1(new_diff) < abs1(old_diff)) && diff_diff > MIN_DIFF_DEGREES)
      || sign(new_diff) != sign(old_diff);
}

long Helm::rot(uangle old_heading, uangle current_heading, unsigned long steer_interval) {
  return (((long) udiff(old_heading, current_heading)) * 1000) / (long) steer_interval;
}
