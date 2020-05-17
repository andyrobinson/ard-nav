#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, Logger *loggerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), windsensor(windsensorp), sail(sailp), logger(loggerp), old_heading(0) {}

void Helm::steer(uangle direction, long steer_time, long steer_interval) {
    long remaining = steer_time;

    char logmsg[22];

    while (remaining > 0) {

      angle current_heading = compass->bearing();
      angle new_rudder_position = new_rudder(direction, current_heading, steer_interval);

      set_rudder(new_rudder_position, current_heading);
      sail->set_position(windsensor->relative());

      timer->wait(steer_interval);
      remaining = remaining - steer_interval;

      sprintf(logmsg, "Hm %4d %4d %8d", new_rudder_position, direction, remaining); logger->msg(logmsg);
    }
}

void Helm::set_rudder(angle new_position, uangle current_heading) {
  rudder->set_position(new_position);
  rudder_position = new_position;
  old_heading = current_heading;
}

angle Helm::new_rudder(uangle direction, uangle current_heading, long steer_interval) {

  angle new_position;

  if (heading_and_turn_ok(direction, old_heading, current_heading)) {
    new_position =  rudder_position; // no change
  } else {
    long desired_rot = udiff(current_heading, direction);
    long actual_rot = rot(old_heading, current_heading, steer_interval);

    if (not_enough_turn(desired_rot, actual_rot)) {
     new_position = rudder_position - sign(desired_rot) * NUDGE_DEGREES; // increase deflection
    }
    else if (too_much_turn(desired_rot, actual_rot)) {
         new_position = rudder_position + sign(desired_rot) * NUDGE_DEGREES; // decrease deflection
    }
    else {
       new_position = rudder_position; // no change
    }
  }

  if (abs1(new_position) > RUDDER_MAX_DISPLACEMENT) {
    new_position = sign(new_position) * RUDDER_MAX_DISPLACEMENT;
  }

  return new_position;
}

bool Helm::not_enough_turn(angle desired_rotation, angle actual_rotation) {
  return (sign(desired_rotation) != sign(actual_rotation)) ||
  (abs1(desired_rotation) - abs1(actual_rotation) > MIN_DIFF_DEGREES);
}

bool Helm::too_much_turn(angle desired_rotation, angle actual_rotation) {
  return (abs1(actual_rotation) - abs1(desired_rotation)) > MIN_DIFF_DEGREES;
}

bool Helm::heading_and_turn_ok(uangle direction, uangle old_heading, uangle current_heading) {
  return (abs1(udiff(direction, current_heading)) <= MIN_DIFF_DEGREES) &&
  (abs1(udiff(current_heading, old_heading)) <= MIN_DIFF_DEGREES);
}

// rotation speed in degrees per second
long Helm::rot(uangle old_heading, uangle current_heading, long steer_interval) {
  return (((long) udiff(old_heading, current_heading)) * 1000) / steer_interval;
}
