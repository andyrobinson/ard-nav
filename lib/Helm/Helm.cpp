#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), windsensor(windsensorp), sail(sailp), rotarypid(rotarypidp), logger(loggerp) {}

void Helm::steer(uangle direction, long steer_time, windrange range) {
    long remaining = steer_time;
    out_of_range_count = 0;
    short increment = 5;

    char logmsg[50];
    sprintf(logmsg, "Steer %4d %8d", direction, steer_time); logger->banner(logmsg);
    if (compass->err_percent() >= 100) {  // both the same underneath
      sprintf(logmsg, "** I2C Failure **"); logger->banner(logmsg);
    }

    while (ok_to_continue(remaining, steer_time, range)) {
      uangle current_heading = compass->bearing();
      if (current_heading == ANGLE_ERROR) {
          rudder_position = 0;
          rudder->set_position(rudder_position);
      } else {
//          rudder_position = rotarypid->calculate(direction, current_heading, STEER_INTERVAL);
          rudder_position = rudder_position + increment;
          if (abs1(rudder_position) > 45) increment = -increment;
          rudder->set_position(rudder_position);
          sprintf(logmsg, ":%3d %3d %3d", direction, current_heading, rudder_position); logger->msg(logmsg);
      }
      timer->wait(STEER_INTERVAL/2);

      sail->set_position(windsensor->relative());
      timer->wait(STEER_INTERVAL/2);

      remaining = remaining - STEER_INTERVAL;
    }
}

bool Helm::ok_to_continue(long time_left, long total_time, windrange range) {
    char logmsg[22];
    uangle abs_wind = windsensor->absolute(compass->bearing());
    bool is_in_range = in_range(abs_wind, range.lower, range.upper) || abs_wind == ANGLE_ERROR;
    (is_in_range) ? out_of_range_count-- : out_of_range_count++;
    out_of_range_count = constrain(out_of_range_count, 0, MAX_OUT_OF_RANGE);

    if (!is_in_range && out_of_range_count >= MAX_OUT_OF_RANGE && time_left > 0) {
        sprintf(logmsg, "Abandon: %3d|%3d|%3d", abs_wind, range.lower, range.upper); logger->banner(logmsg);
        return false;
    }
    return (time_left > 0);
}
