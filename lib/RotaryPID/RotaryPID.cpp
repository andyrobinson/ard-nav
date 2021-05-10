#include "RotaryPID.h"

using namespace Angle;
using namespace Utility;

RotaryPID::RotaryPID() {}

RotaryPID::RotaryPID(Switches *switchesp, Logger *loggerp):
  switches(switchesp), logger(loggerp) {}

angle RotaryPID::calculate(uangle desired_heading, uangle current_heading, long interval_ms) {
    return 0;
}

