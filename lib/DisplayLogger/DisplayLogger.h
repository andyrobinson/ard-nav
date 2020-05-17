#ifndef DisplayLogger_h
#define DisplayLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"

#define GPS_WAIT_MILLIS 10
#define BASE10 10

class DisplayLogger : public Logger {
    public:
      DisplayLogger();
      DisplayLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp);
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      void messageAt(int y, char *msg);
      void append_double5pl(char *buf, double dbl);
      void append_double1pl(char *buf, double dbl);
      void append_int(char *buf, int i);
      void append_digit(char *buf, int i);
      boolean time_to_change_message();

      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      gpsResult gpsReading;
      char destination;
      char tack;
      unsigned long last_log_time;
      boolean hold_last_message;

};

#endif
