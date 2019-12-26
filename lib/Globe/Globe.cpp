#include "Globe.h"
#include "Angle.h"
#include "Position.h"

using namespace Angle;
using namespace Position;

Globe::Globe() {}

double distance_between(position *start, position *finish) {
    return 0.0;
}

angle bearing(position *start, position *finish) {
    return 0;
}

position new_position(position *start, angle bearing, double distance) {
    position new_pos = {1.0, 1.0};
    return new_pos;
}

double to_radians(angle angle) {
    return 0.0;
}
