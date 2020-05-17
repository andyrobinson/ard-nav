#ifndef LoggerStub_h
#define LoggerStub_h

#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "cstdio"

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

template <typename T> T F(T value) {
 return value;
}

#endif
