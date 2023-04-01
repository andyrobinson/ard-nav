#include "SDBackedRoute.h"

SDBackedRoute::SDBackedRoute(){};
SDBackedRoute::SDBackedRoute(SDClass sd){};

void SDBackedRoute::begin(route *rt, bool cold_start){};

bool SDBackedRoute::done(){
    return false;
}

waypoint *SDBackedRoute::next() {
    return &wp;
}

