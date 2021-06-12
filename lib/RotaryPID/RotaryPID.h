#ifndef RotaryPID_h
#define RotaryPID_h

#include "Utility.h"
#include "Angle.h"
#include "Switches.h"
#include "Logger.h"

#define KP 0.5
#define KI 0.8
#define KD 0.1

using namespace Angle;

class RotaryPID
{
  public:
      RotaryPID();
      RotaryPID(float limit_param, Switches *switchesp, Logger *loggerp);
      angle calculate(uangle desired_heading, uangle current_heading, long interval_ms);

  private:
    uangle last_heading;
    Switches *switches;
    Logger *logger;
    float integral_term, limit, output;
    float Kp, Ki, Kd;

    float clip(float value, float limit);
    void set_constants();

};

#endif
