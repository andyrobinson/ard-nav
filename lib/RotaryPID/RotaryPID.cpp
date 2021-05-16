#include "RotaryPID.h"

// based on work by Brett Beauregard - see
// http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

using namespace Angle;
using namespace Utility;

RotaryPID::RotaryPID() {}

RotaryPID::RotaryPID(float limit_param, float kp, float ki, float kd):
  limit(limit_param), Kp(kp), Ki(ki), Kd(kd), integral_term(0.0), last_heading(0) {}

angle RotaryPID::calculate(uangle desired_heading, uangle current_heading, long interval_ms) {
    float sample_time_sec = ((float) interval_ms)/1000;
    float error = (float) udiff(current_heading,desired_heading);

    integral_term += (Kp * sample_time_sec * error);
    integral_term = clip(integral_term, limit);

    float diff_input = (float) udiff(last_heading,current_heading);

    output = (Kp * error) + integral_term - ((Kd / sample_time_sec) * diff_input);
    output = clip(output, limit);

    last_heading = current_heading;
    return angle(-output); // rudder sign is opposite to rotation direction
}

float RotaryPID::clip(float value, float limit) {
    return max1(min1(limit, value), -limit);
}


