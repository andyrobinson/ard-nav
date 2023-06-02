#ifndef SatComm_h
#define SatComm_h

#include "IridiumSBD.h"
#include "Timer.h"

#define SAT_LOG_WINDOWS_MINS 5

class SatComm {
    public:
      SatComm();
      SatComm(IridiumSBD *modemp, Timer *timer);

      virtual void begin();
      bool steer_log_or_continue();

      static const uint8_t log_hours[8];
      static const uint8_t log_minutes[1];

    private:

      bool inArray(uint8_t val, const uint8_t *arr, int length);
      bool inWindow(uint8_t val, const uint8_t *arr, int length);

      IridiumSBD *modem;
      Timer *timer;

};

#endif
