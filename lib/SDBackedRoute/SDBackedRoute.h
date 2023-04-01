#ifndef SDBackedRoute_h
#define SDBackedRoute_h

#include "Route.h"
#include "SD.h"

using namespace SDLib;

class SDBackedRoute : public Route {
    public:
      SDBackedRoute();
      SDBackedRoute(SDClass sd);

      virtual void begin(route *rt, bool cold_start) ;
      virtual bool done();
      virtual waypoint *next();

    private:
      waypoint wp;

};

#endif
