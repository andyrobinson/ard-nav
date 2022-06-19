#ifndef SerialLogger_h
#define SerialLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "Battery.h"

#define GPS_WAIT_MILLIS 10

class SerialLogger : public Logger {
    public:
      SerialLogger();
      SerialLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp);
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      Battery *battery;
      gpsResult gpsReading;
      char destination;
      char tack;
};

#endif
