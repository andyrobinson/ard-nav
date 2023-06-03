#ifndef SatComm_h
#define SatComm_h

#include "IridiumSBD.h"
#include "Timer.h"
#include "Gps.h"
#include "Logger.h"
#include "Utility.h"
#include "Battery.h"
#include "Compass.h"

#define SAT_LOG_WINDOWS_MINS 5
#define SAT_LOG_RECORD_SIZE 24
#define SAT_GPS_WAIT_MILLIS 30000

using namespace Utility;

class SatComm {
    public:
      SatComm();
      SatComm(IridiumSBD *modemp, Timer *timerp, Gps *gpsp, Battery *batteryp, Compass *compassp, Logger *loggerp);

      virtual void begin();
      bool steer_log_or_continue();
      void set_dest(char *label);

      static const uint8_t log_hours[8];
      static const uint8_t log_minutes[1];

    private:

      char charOrSpace(char ch);
      bool inArray(uint8_t val, const uint8_t *arr, int length);
      bool inWindow(uint8_t val, const uint8_t *arr, int length);
      void setData();

      IridiumSBD *modem;
      Timer *timer;
      Gps *gps;
      Battery *battery;
      Compass *compass;
      Logger *logger;

      uint8_t send_buffer[50];
      gpsResult gps_data;
      char wp_label[2];
};

#endif
