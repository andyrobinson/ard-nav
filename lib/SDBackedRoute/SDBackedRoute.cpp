#include "SDBackedRoute.h"

SDBackedRoute::SDBackedRoute(){};
SDBackedRoute::SDBackedRoute(SDClass *sdp):sd(sdp),filepath_buffer(""){};

void SDBackedRoute::begin(route *rt, bool cold_start){
      waypoints = rt->waypoints;
      route_length = rt->length;
      wp_index = 0;
      sprintf(filepath_buffer,"%s.dat",rt->label);
      if(cold_start) {
          sd->remove(filepath_buffer);
          File routeFile = sd->open(filepath_buffer, FILE_WRITE);
          twoDigitsToBuffer(0);
          routeFile.write(digit_buffer, 2);
          routeFile.close();
      }
};

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
   else
        return &waypoints[wp_index++];
}

