#include "Compass.h"

Compass::Compass(Boat *boatp): boat(boatp) {}

uangle Compass::bearing() {
    return boat->bearing();
}

int Compass::err_percent() {
    return 0;
}


