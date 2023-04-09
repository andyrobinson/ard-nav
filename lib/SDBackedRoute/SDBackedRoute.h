#ifndef SDBackedRoute_h
#define SDBackedRoute_h

#include "Route.h"
#include "SD.h"
#include "Arduino.h"

using namespace SDLib;

class SDBackedRoute : public Route {
    public:
      SDBackedRoute();
      SDBackedRoute(SDClass *sdp);

      virtual void begin(route *rt, bool cold_start) ;
      virtual bool done();
      virtual waypoint *next();

    private:
      void twoDigitsToBuffer(int val);
      void do_cold_start();
      void do_warm_start();

      char filepath_buffer[13];
      uint8_t digit_buffer[2];
      SDClass *sd;
      waypoint *waypoints;
      int route_length;
      int wp_index;
};

#endif
