#ifndef SatComm_h
#define SatComm_h

#include "IridiumSBD.h"
#include "Timer.h"
#include "Gps.h"
#include "Logger.h"
#include "Utility.h"
#include "Battery.h"
#include "Compass.h"
#include "Arduino.h"
#include "time.h"

#define SAT_LOG_WINDOWS_MINS 5
#define SAT_LOG_RECORD_SIZE 34
#define SAT_GPS_WAIT_MILLIS 1000 // cannot wait for GPS
#define SAT_MILLIS_IN_MINUTE 60000
#define SAT_TIME_SET_INTERVAL_MINS 10

using namespace Utility;

class SatComm {
    public:
      SatComm();
      SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Battery *batteryp, Compass *compassp, Logger *loggerp);

      virtual void begin();
      bool steer_log_or_continue();
      void set_dest(char label);

      static const uint8_t log_hours[24];
      static const uint8_t log_minutes[6];

    private:

      char charOrSpace(char ch);
      bool isHourtoLog(uint8_t val, const uint8_t *arr, int length);
      bool isMinutetoLog(uint8_t val, const uint8_t *arr, int length);
      void insertLogDataIntoBuffer();
      bool noRecentSuccess();
      bool recentlyAttemptedToLog();
      void tryModemTime();
      int mins_left_in_window(uint8_t val, const uint8_t *arr, int length);

      IridiumSBD *modem;
      Timer *timer;
      Gps *gps;
      Battery *battery;
      Compass *compass;
      Logger *logger;

      uint8_t send_buffer[50];
      gpsResult gps_data;
      char wp_label;
      unsigned long last_log;
      unsigned long last_modem_attempt_time;
      unsigned long last_attempt;

};

#endif
