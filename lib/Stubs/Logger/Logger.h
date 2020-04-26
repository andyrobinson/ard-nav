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
    Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp);
    void begin();
    void msg(char *message);
    void banner(char *message);

  private:
    Gps *gps;
    WindSensor *windsensor;
    Compass *compass;
    gpsResult gpsReading;

};

template <typename T> T F(T value) {
 return value;
}

#endif
