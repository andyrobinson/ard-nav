#include "SatComm.h"

SatComm::SatComm():result(true),steer_count(0),waypoint(' '){};

void SatComm::begin(){
    result = true;
};

bool SatComm::steer_log_or_continue() {
    steer_count++;
    return result;
}

void SatComm::set_dest(char label) {
    waypoint=label;
}

void SatComm::reset() {
    steer_count = 0;
}
