#include "SatComm.h"

SatComm::SatComm(){};
SatComm::SatComm(Boat *boatp):boat(boatp){};

void SatComm::begin(){};

bool SatComm::steer_log_or_continue() {
    return true;  // may want to do some logging eventually
} 

void SatComm::set_dest(char label) {}

