#ifndef SerialLogger_h
#define SerialLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "Battery.h"
#include "Timer.h"
#include "time.h"

#define GPS_WAIT_MILLIS 10

class SerialLogger : public Logger {
    public:
      SerialLogger();
      SerialLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp, Timer *timerp);
      virtual void begin();
      virtual void msg(const char *message);
      virtual void banner(const char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);
      virtual void write_version(int major, int minor);

    private:
      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      Battery *battery;
      Timer *timer;
      gpsResult gpsReading;
      char destination;
      char tack;
};

#endif
