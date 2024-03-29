#ifndef MultiLogger_h
#define MultiLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"

class MultiLogger : public Logger {
    public:
      MultiLogger();
      MultiLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Logger **loggersp, int num);
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
      Logger **loggers;
      int numLoggers;
};

#endif
