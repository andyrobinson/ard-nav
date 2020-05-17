#ifndef Logger_h
#define Logger_h

#include "WindSensor.h"
#include "Compass.h"
#include "Position.h"
#include "Gps.h"
#include "Angle.h"

using namespace Angle;
using namespace Position;

#define GPS_WAIT_MILLIS 10
#define BASE10 10

class Logger
{
  public:
    Logger();
    Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger *loggersp, int num);
    void begin();
    void msg(char *message);
    void banner(char *message);
    void setdest(char destletter);
    void settack(char tackletter);

  private:
    Gps *gps;
    WindSensor *windsensor;
    Compass *compass;
    gpsResult gpsReading;
    Logger *loggers;
    char destination;
    char tack;
    int numLoggers;
};

#endif
