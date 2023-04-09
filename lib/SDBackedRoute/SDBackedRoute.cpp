#include "SDBackedRoute.h"

SDBackedRoute::SDBackedRoute(){};
SDBackedRoute::SDBackedRoute(SDClass *sdp):sd(sdp),filepath_buffer(""){};

void SDBackedRoute::begin(route *rt, bool cold_start){
      waypoints = rt->waypoints;
      route_length = rt->length;
      wp_index = 0;
      sprintf(filepath_buffer,"%s.dat",rt->label);

      if (cold_start || !sd->exists(filepath_buffer))
        do_cold_start();
      else
        do_warm_start();
};

void SDBackedRoute::do_warm_start() {
    File routeFile = sd->open(filepath_buffer, FILE_READ);
    char digit1, digit2 = '0';
    while (routeFile.available()) {
      digit1 = digit2;
      digit2 = routeFile.read();
    }
    routeFile.close();
    wp_index = ((digit1 - '0')) * 10 + (digit2 - '0');
    wp_index = constrain(wp_index,0,route_length-1); // for safety
}

void SDBackedRoute::do_cold_start() {
  if (sd->exists(filepath_buffer))
      sd->remove(filepath_buffer);
}

bool SDBackedRoute::done() {
    return (wp_index == route_length);
}

void SDBackedRoute::twoDigitsToBuffer(int val) {
  digit_buffer[0] = '0' + val / 10;
  digit_buffer[1] = '0' + val % 10;
}

waypoint *SDBackedRoute::next() {
   if (done())
        return &waypoints[wp_index-1];
   else {
        File routeFile = sd->open(filepath_buffer, FILE_WRITE);
        twoDigitsToBuffer(wp_index);
        routeFile.write(digit_buffer, 2);
        routeFile.close();

        return &waypoints[wp_index++];
   }
}

