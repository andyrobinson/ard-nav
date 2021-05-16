#include "Tacker.h"
#include "Angle.h"
#include "Utility.h"
#include "math.h"

using namespace Angle;
using namespace Utility;

Tacker::Tacker() {}

Tacker::Tacker(Helm *helmp, Compass *compassp, WindSensor *windsensorp, Logger *loggerp):
  helm(helmp), compass(compassp), windsensor(windsensorp), logger(loggerp) {}

void Tacker::steer(uangle direction, long steer_time) {

  angle wind_diff = udiff(windsensor->absolute(compass->bearing()), direction);
  char logmsg[10]="Straight";

  if (abs1(wind_diff) >= TACKER_NO_GO_LIMIT) {
    // no tack
    logger->settack('0');
    logger->msg(logmsg);
    helm->steer(direction, steer_time);
  }
  else {
    short offset = TACKER_NO_GO_LIMIT - abs1(wind_diff);

    // first tack
    logger->settack('1');
    sprintf(logmsg,"Tack1");logger->banner(logmsg);
    uangle tack_direction = uadd(direction, sign(wind_diff) * offset);
    long tack_time = round(steer_time * cos(to_radians((double) offset)));
    helm->steer(tack_direction, tack_time);

    // second tack
    logger->settack('2');
    tack_direction = uadd(tack_direction, - sign(wind_diff) * TACKER_NO_GO_LIMIT * 2);
    tack_time = round(steer_time * sin(to_radians((double) offset)));
    if (tack_time > MIN_TACK_MS) {
        sprintf(logmsg,"Tack2");logger->banner(logmsg);
        helm->steer(tack_direction, tack_time);
    } else {
        sprintf(logmsg,"No Tack2");logger->banner(logmsg);
    }
  }
}
