#include "Tacker.h"
#include "Utility.h"
#include "math.h"

using namespace Angle;
using namespace Utility;
using namespace Windrange;

Tacker::Tacker() {}

Tacker::Tacker(IHelm *helmp, Compass *compassp, WindSensor *windsensorp, Logger *loggerp):
  helm(helmp), compass(compassp), windsensor(windsensorp), logger(loggerp) {}

void Tacker::steer(uangle direction, long steer_time) {

  char logmsg[20]="Straight";
  angle current_heading = compass->bearing();
  uangle abs_wind = windsensor->absolute(current_heading);
  angle wind_diff = udiff(abs_wind, direction);

  if (abs1(wind_diff) >= TACKER_NO_GO_LIMIT || current_heading == ANGLE_ERROR || abs_wind == ANGLE_ERROR) {
    // no tack
    logger->settack('0');
    helm->steer(direction, steer_time,
        {uadd(direction, (angle) WIND_RANGE_NO_GO_LIMIT),
         uadd(direction, (angle) -WIND_RANGE_NO_GO_LIMIT)});
  }
  else {
    angle wind_sign = wind_diff == 0 ? 1 : sign(wind_diff);
    short offset = TACKER_NO_GO_LIMIT - abs1(wind_diff);

    // first tack
    logger->settack('1');
    uangle tack_direction = uadd(direction, wind_sign * offset);
    long tack_time = round(steer_time * cos(to_radians((double) offset)));
    helm->steer(tack_direction, tack_time, set_range(tack_direction, wind_sign));

    // second tack
    logger->settack('2');
    tack_direction = uadd(tack_direction, - wind_sign * TACKER_NO_GO_LIMIT * 2);
    tack_time = round(steer_time * sin(to_radians((double) offset)));
    if (tack_time > MIN_TACK_MS) {
        helm->steer(tack_direction, tack_time, set_range(tack_direction, -wind_sign));
    } else {
        sprintf(logmsg,"No Tack2");logger->banner(logmsg);
    }
  }
}

windrange Tacker::set_range(uangle direction, short sign) {
    if (sign > 0)
      return {uadd(direction, 180), uadd(direction, -WIND_RANGE_NO_GO_LIMIT)};
    else
      return {uadd(direction, WIND_RANGE_NO_GO_LIMIT), uadd(direction, 180)};
}
