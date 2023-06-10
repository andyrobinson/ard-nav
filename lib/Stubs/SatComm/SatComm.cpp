#include "SatComm.h"

SatComm::SatComm():result(true),steer_count(0){};

void SatComm::begin(){
    result = true;
};

bool SatComm::steer_log_or_continue() {
    steer_count++;
    return result;
}

void SatComm::set_dest(char *label) {
    for (int i;i<3;i++) {
        dest[i] = label[i];
    }
}

void SatComm::reset() {
    steer_count = 0;
}
