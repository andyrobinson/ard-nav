#include "Compass.h"

Compass::Compass(Boat *boatp): boat(boatp) {}

uangle Compass::bearing() {
    return boat->bearing();
}

