#ifndef Navigator_h
#define Navigator_h

#include "Tacker.h"
#include "Waypoint.h"
#include "Gps.h"
#include "Globe.h"
#include "Logger.h"
#include "Utility.h"
#include "Angle.h"
#include "Arduino.h"

#define MAX_GPS_WAIT_FOR_FIX 1000
#define MAX_STEER_TIME  36000.0 // beware maximum tack time in small ponds!
#define MIN_STEER_TIME 5000.0

using namespace Angle;
using namespace Position;

class Navigator
{
  public:
    Navigator();
    Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep, Logger *loggerp);
    void sailto(waypoint destination);

  private:
    Tacker *tacker;
    Gps *gps;
    Globe *globe;
    Logger *logger;
    bool arrived(position destination, position current);
};

#endif
