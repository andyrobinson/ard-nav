#ifndef SatComm_h
#define SatComm_h

#include "IridiumSBD.h"
#include "Timer.h"
#include "Gps.h"
#include "Logger.h"
#include "Utility.h"

#define SAT_LOG_WINDOWS_MINS 5
#define SAT_LOG_RECORD_SIZE 8
#define SAT_GPS_WAIT_MILLIS 30000

using namespace Utility;

class SatComm {
    public:
      SatComm();
      SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Logger *loggerp);

      virtual void begin();
      bool steer_log_or_continue();

      static const uint8_t log_hours[8];
      static const uint8_t log_minutes[1];

    private:

      bool inArray(uint8_t val, const uint8_t *arr, int length);
      bool inWindow(uint8_t val, const uint8_t *arr, int length);

      uint8_t send_buffer[50];

      IridiumSBD *modem;
      Timer *timer;
      Gps *gps;
      Logger *logger;

      gpsResult gps_data;
};

#endif
