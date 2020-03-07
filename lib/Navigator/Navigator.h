#ifndef Navigator_h
#define Navigator_h

#include "Tacker.h"
#include "Position.h"
#include "Gps.h"
#include "Globe.h"
#include "Utility.h"
#include "Angle.h"

#define MAX_GPS_WAIT_FOR_FIX 60000
#define STEER_CHECK_INTERVAL 60000
#define MAX_STEER_TIME 3600000.0

using namespace Angle;
using namespace Position;

class Navigator
{
  public:
    Navigator();
    Navigator(Tacker *tackerp, Gps *gpsp, Globe *globep);
    void sailto(position destination);

  private:
    Tacker *tacker;
    Gps *gps;
    Globe *globe;
    bool arrived(position destination, position current);
};

#endif
