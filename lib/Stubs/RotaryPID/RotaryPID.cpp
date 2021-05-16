#include "RotaryPID.h"

using namespace Angle;

RotaryPID::RotaryPID():current_position(0),list_size(1),result_list{0} {}

angle RotaryPID::calculate(uangle desired_heading, uangle current_heading, long interval_ms) {
   angle result = result_list[current_position];
   if (current_position + 1 < list_size) current_position++;
   return result;
}

void RotaryPID::set_results(angle *results, int size) {
    result_list = results;
    list_size = size;
    current_position = 0;
}
