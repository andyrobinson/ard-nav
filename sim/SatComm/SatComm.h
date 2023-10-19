#ifndef SatCommSim_h
#define SatCommSim_h

#include "Boat.h"
#include "Arduino.h"

class SatComm {
    public:
      SatComm();
      SatComm(Boat *boatp);

      virtual void begin();
      bool steer_log_or_continue();
      void set_dest(char label);

    private:
      Boat *boat;
};

#endif
