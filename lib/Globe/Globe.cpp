#include "Globe.h"
#include "Angle.h"
#include "Position.h"

using namespace Angle;
using namespace Position;

Globe::Globe() {}

double distance_between(Position *start, Position *finish) {
    return 0.0;
}

Angle bearing(Position *start, Position *finish) {
    return 0;
}

Position new_position(Position *start, angle bearing, double distance) {
    Position new_pos = {1.0, 1.0};
    return new_pos;
}

double to_radians(Angle angle) {
    return 0.0;
}
