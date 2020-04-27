#ifndef Navigator_h
#define Navigator_h

#include "Tacker.h"
#include "Waypoint.h"
#include "Gps.h"
#include "Globe.h"
#include "Logger.h"
#include "Utility.h"
#include "Angle.h"

#define MAX_GPS_WAIT_FOR_FIX 60000
#define STEER_CHECK_INTERVAL 200
#define MAX_STEER_TIME  60000.0 //3600000.0
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
