#include "Helm.h"
#include "Angle.h"
#include "Utility.h"

using namespace Angle;
using namespace Utility;

Helm::Helm():rudder_position(0) {}

Helm::Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, Logger *loggerp):
  rudder_position(0),rudder(rudderp), compass(compassp), timer(timerp), windsensor(windsensorp), sail(sailp), rotarypid(rotarypidp), logger(loggerp), old_heading(0) {}

void Helm::steer(uangle direction, long steer_time, windrange range) {

    long remaining = steer_time;
    char logmsg[50] = "stuff";

    sprintf(logmsg, "Steer %4d %8d", direction, steer_time); logger->banner(logmsg);

    angle TEMP_RUDDER = 25;
    angle TEMP_RELATIVE_WIND = 100;
    int SAIL_COUNT = 0;

    if (compass->err_percent() >= 100 || windsensor->err_percent() >=100) {
      sprintf(logmsg, "** I2C Failure **"); logger->banner(logmsg);
      while (true) {};
    }

    while (remaining > 0) { // && wind_in_range(range)) {
      long iteration_time = STEER_INTERVAL;
      logger->msg("while");
      timer->wait(10);
      iteration_time = iteration_time - 10;
      angle current_heading = compass->bearing();
      logger->msg("*1");
      angle new_rudder_position = rotarypid->calculate(direction, current_heading, STEER_INTERVAL);
      logger->msg("*2");

       // set_rudder(new_rudder_position, current_heading);
       // sail->set_position(windsensor->relative());
       set_rudder(TEMP_RUDDER, current_heading);
       logger->msg("*3");

       SAIL_COUNT += 1;
       if (SAIL_COUNT == 10) {
         SAIL_COUNT = 0;
         timer->wait(200);
         iteration_time = iteration_time - 200;
         logger->msg("start sail");
         sail->set_position(TEMP_RELATIVE_WIND);
         timer->wait(400);
         iteration_time = iteration_time - 400;
         logger->msg("finish sail");
         TEMP_RELATIVE_WIND = -TEMP_RELATIVE_WIND;
       }

      // additional logging to stress SD writing
      while (iteration_time > 0) {
        logger->msg("*4");
        timer->wait(20);
        long turnrate = rot(old_heading, current_heading, STEER_INTERVAL);
        logger->msg("*5");
        sprintf(logmsg, "%8d %3d %8d %2d", turnrate, new_rudder_position, remaining, SAIL_COUNT); logger->msg(logmsg);
        timer->wait(20);
        iteration_time = iteration_time - 50;
      }

      TEMP_RUDDER = -TEMP_RUDDER;

      remaining = remaining - STEER_INTERVAL;
      logger->msg("end while");

    }

}

void Helm::set_rudder(angle new_position, uangle current_heading) {
  rudder->set_position(new_position);
  rudder_position = new_position;
  old_heading = current_heading;
}

// rotation speed in degrees per second
long Helm::rot(uangle old_heading, uangle current_heading, long steer_interval) {
  return (((long) udiff(old_heading, current_heading)) * 1000) / steer_interval;
}

bool Helm::wind_in_range(windrange range) {
    char logmsg[22];
    uangle abs_wind = windsensor->absolute(compass->bearing());
    if (in_range(abs_wind, range.lower, range.upper)) return true;
    sprintf(logmsg, "Abandon: %3d,%3d,%3d", abs_wind, range.lower, range.upper); logger->banner(logmsg);
    return false;
}
