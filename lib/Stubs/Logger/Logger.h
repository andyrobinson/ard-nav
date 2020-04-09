#ifndef LoggerStub_h
#define LoggerStub_h

#include "Position.h"
#include "Gps.h"
#include "Angle.h"

using namespace Angle;
using namespace Position;

class Logger
{
  public:
    Logger();
    void begin();
    void info(gpsResult *gps_result, angle wind, uangle bearing, String message);
    void msg(String message);
};

#endif
