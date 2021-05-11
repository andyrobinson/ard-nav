#ifndef RotaryPID_h
#define RotaryPID_h

#include "Utility.h"
#include "Angle.h"
#include "Switches.h"

#define KP 1.0
#define KI 1.0
#define KD 1.0

using namespace Angle;

class RotaryPID
{
  public:
      RotaryPID();
      RotaryPID(float limit_param, Switches *switchesp);
      angle calculate(uangle desired_heading, uangle current_heading, long interval_ms) ;

  private:
    uangle last_heading;
    float integral_term, limit, output;
    Switches *switches;

    float clip(float value, float limit);

};

#endif
