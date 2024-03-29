#include "RotaryPID.h"

// based on work by Brett Beauregard - see
// http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

using namespace Angle;
using namespace Utility;

RotaryPID::RotaryPID() {}

RotaryPID::RotaryPID(float limit_param, Switches *switchesp):
  limit(limit_param), Kp(KP), Ki(KI), Kd(KD), integral_term(0.0), last_heading(0), output(0), switches(switchesp) {}

angle RotaryPID::calculate(uangle desired_heading, uangle current_heading, long interval_ms) {
    set_constants();

    float sample_time_sec = ((float) interval_ms)/1000;
    float error = (float) udiff(current_heading,desired_heading);
    float max_deflection =  MAX_DEFLECTION_PER_SECOND * sample_time_sec;

    last_output = output;

    integral_term += (Kp * sample_time_sec * error);
    integral_term = constrain(integral_term, -limit, limit);

    float diff_input = (float) udiff(last_heading,current_heading);

    output = (Kp * error) + integral_term - ((Kd / sample_time_sec) * diff_input);
    output = constrain(output, last_output-max_deflection, last_output+max_deflection);; // rudder cannot do entire sweep in interval
    output = constrain(output, -limit, limit); // rudder end points

    last_heading = current_heading;
    return angle(-output); // rudder sign is opposite to rotation direction
}

void RotaryPID::set_constants() {
  float percent = switches->dial_percent();
  Kp = KP * 1.5;
  Ki = KI * 2;
  // 0-50 percent gives (0.5 - 1.0), 50-100 percent gives (1.0 - 5.0)
  Kd = 0.5 + (min1(percent, 50.0f)/100.0f) + (max1((percent - 50.0f),0.0f)/12.5f);

}



