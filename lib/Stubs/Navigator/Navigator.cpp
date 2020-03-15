#include "Navigator.h"

Navigator::Navigator():current_position(0) {}

void Navigator::sailto(position destination) {
  positions[current_position++] = destination;
}

position *Navigator::get_waypoints() {
    return positions;
}

void Navigator::reset() {
    current_position = 0;
}
