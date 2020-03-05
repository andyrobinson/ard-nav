#include "Globe.h"
#include "Angle.h"
#include "Position.h"
#include "math.h"

using namespace Angle;
using namespace Position;

Globe::Globe() {}

// all calculations based on http://www.movable-type.co.uk/scripts/latlong.html
// note one degree of latitude is about 111 km
// Therefore the 5th decimal place is the one that represents metres

double Globe::distance_between(position *start, position *finish) {
  double start_lat = to_radians(start -> latitude);
  double end_lat = to_radians(finish->latitude);
  double diff_lat = to_radians(finish->latitude - start->latitude);
  double diff_long = to_radians(finish->longitude - start->longitude);

  double square_half_chord_length = pow(sin(diff_lat/2.0),2.0) + cos(start_lat)  *  cos(end_lat) * pow(sin(diff_long/2.0),2.0);
  double angular_distance = 2.0 * asin(sqrt(square_half_chord_length));
  return EARTH_RADIUS_METRES * angular_distance;
}

uangle Globe::bearing(position *start, position *finish) {
  double start_lat = to_radians(start->latitude);
  double end_lat = to_radians(finish->latitude);
  double diff_long=to_radians(finish->longitude - start -> longitude);
  double y = sin(diff_long) * cos(end_lat);
  double x = cos(start_lat) * sin(end_lat) - (sin(start_lat) * cos(end_lat) * cos(diff_long));
  double bearing = to_degrees(atan2(y,x));
  return (uangle)round((bearing > 0.0) ? bearing :  bearing + 360.0);
}

position Globe::new_position(position *start, uangle bearing, double distance) {
  double start_lat = to_radians(start->latitude);
  double start_long = to_radians(start->longitude);
  double bearing_in_rads = to_radians((double) bearing);
  double distance_in_rads = distance/EARTH_RADIUS_METRES;
  double new_lat = asin(sin(start_lat) * cos(distance_in_rads) + cos(start_lat)*sin(distance_in_rads)*cos(bearing_in_rads));
  double new_long = start_long + atan2(sin(bearing_in_rads)*sin(distance_in_rads)*cos(start_lat),cos(distance_in_rads) - sin(start_lat)*sin(new_lat));

  return {to_degrees(new_lat),to_degrees(new_long),0.0};
}
