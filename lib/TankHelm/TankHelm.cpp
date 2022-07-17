#include "TankHelm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

TankHelm::TankHelm():rudder_position(0) {}

TankHelm::TankHelm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), windsensor(windsensorp), sail(sailp), rotarypid(rotarypidp), logger(loggerp), old_heading(0) {}

void TankHelm::steer(uangle direction, long steer_time, windrange range) {
    long remaining = steer_time;
    angle new_rudder_position;
    angle wind_position = 0;
    uangle wind_increment = 10;

    char logmsg[50];
    sprintf(logmsg, "Steer %4d %8d", direction, steer_time); logger->banner(logmsg);
    if (compass->err_percent() >= 100) {  // both the same underneath
      sprintf(logmsg, "** I2C Failure **"); logger->banner(logmsg);
    }

    while (remaining > 0) {

      angle current_heading = compass->bearing();
      rotarypid->calculate(direction, current_heading, STEER_INTERVAL); // for lols
      new_rudder_position = new_rudder_position == RUDDER_VALUE ? -RUDDER_VALUE : RUDDER_VALUE;
      set_rudder(new_rudder_position, current_heading);

      timer->wait(STEER_INTERVAL/2);

      sail->set_position(wind_position);
      wind_position = wind_position + wind_increment;
      if (abs1(wind_position) >= RELATIVE_WIND_MAX) wind_increment = -wind_increment;

      timer->wait(STEER_INTERVAL/2);

      remaining = remaining - STEER_INTERVAL;

      long turnrate = rot(old_heading, current_heading, STEER_INTERVAL);
      sprintf(logmsg, "%8d %3d %8d", turnrate, new_rudder_position, remaining); logger->msg(logmsg);
    }

}

void TankHelm::set_rudder(angle new_position, uangle current_heading) {
  rudder->set_position(new_position);
  rudder_position = new_position;
  old_heading = current_heading;
}

// rotation speed in degrees per second
long TankHelm::rot(uangle old_heading, uangle current_heading, long steer_interval) {
  return (((long) udiff(old_heading, current_heading)) * 1000) / steer_interval;
}
