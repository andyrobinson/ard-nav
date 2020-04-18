#include "Navigator.h"

Navigator::Navigator():current_position(0) {}

void Navigator::sailto(waypoint destination) {
  waypoints[current_position++] = destination;
}

waypoint *Navigator::get_waypoints() {
    return waypoints;
}

void Navigator::reset() {
    current_position = 0;
}
