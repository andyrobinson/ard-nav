#ifndef SDLogger_h
#define SDLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "Utility.h"

#define TEST_LOG_FILE 20001l
#define CHIP_SELECT 4
#define BASE10 10
#define LOG_INTERVAL 300ul // may affect battery life
#define GPS_WAIT_MILLIS 10

class SDLogger : public Logger {
    public:
      SDLogger();
      SDLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp);
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      void print_line(char *message, char *msgprefix);
      void calculate_filename(char *filename, long unix_ts);
      boolean sd_time_to_log();

      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      gpsResult gpsReading;
      char destination;
      char tack;
      char logfile[13];
      unsigned long sd_last_log_time;
};

#endif
