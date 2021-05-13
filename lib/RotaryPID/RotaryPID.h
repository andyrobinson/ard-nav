#ifndef RotaryPID_h
#define RotaryPID_h

#include "Utility.h"
#include "Angle.h"
#include "Switches.h"

#define KP 0.5
#define KI 0.5
#define KD 0.5

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
