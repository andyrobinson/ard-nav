#ifndef RotaryPID_h
#define RotaryPID_h

#include "Utility.h"
#include "Angle.h"
#include "Logger.h"
#include "Switches.h"

#define MIN_HEADING_AND_ROT 5
#define NUDGE_DEGREES 4
#define STEER_INTERVAL 370

using namespace Angle;

class RotaryPID
{
  public:
      RotaryPID();
      RotaryPID(Switches *switchesp, Logger *loggerp);
      angle calculate(uangle desired_heading, uangle current_heading, long interval_ms) ;

  private:
    uangle last_heading;
    Switches *switches;
    Logger *logger;
};

#endif
