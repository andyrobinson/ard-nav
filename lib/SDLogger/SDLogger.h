#ifndef SDLogger_h
#define SDLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "Battery.h"
#include "Utility.h"
#include "Switches.h"
#include "Timer.h"
#include "time.h"

#define PRE_JAN1_2000_TS 15000l
#define CHIP_SELECT 4
#define BASE10 10
#define LOG_INTERVAL 5000ul // may affect battery life
#define GPS_WAIT_MILLIS 10
#define LOG_BANNER_LENGTH 500

class SDLogger : public Logger {
    public:
      SDLogger();
      SDLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp, Switches *switchesp, Timer *timerp, long ofilename=0);
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);
      virtual void write_version(int major, int minor);

    private:
      void print_line(char *message, char* prefix);
      void calculate_filename(char *filename, long unix_ts);
      boolean sd_time_to_log();

      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      Battery *battery;
      Switches *switches;
      Timer *timer;
      gpsResult gpsReading;
      char destination;
      char tack;
      char logfile[13];
      char banner_msg[LOG_BANNER_LENGTH];
      int banner_space;
      unsigned long sd_last_log_time;
      long ofilename;

};

#endif
