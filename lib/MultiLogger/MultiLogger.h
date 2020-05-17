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
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      Logger **loggers;
      int numLoggers;
};

#endif
