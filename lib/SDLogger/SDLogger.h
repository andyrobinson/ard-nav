#ifndef SDLogger_h
#define SDLogger_h

#include "Logger.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Gps.h"
#include "Battery.h"
#include "Utility.h"

#define JAN1_2000_TS 15778l
#define CHIP_SELECT 4
#define BASE10 10
#define LOG_INTERVAL 5000ul // may affect battery life
#define GPS_WAIT_MILLIS 10
#define LOG_BANNER_LENGTH 200

class SDLogger : public Logger {
    public:
      SDLogger();
      SDLogger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Battery *batteryp, long ofilename=0);
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      void print_line(char *message);
      void calculate_filename(char *filename, long unix_ts);
      boolean sd_time_to_log();

      Gps *gps;
      WindSensor *windsensor;
      Compass *compass;
      Battery *battery;
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
