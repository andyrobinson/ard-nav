#ifndef SatComm_h
#define SatComm_h

#include "IridiumSBD.h"

class SatComm {
    public:
      SatComm();
      SatComm(IridiumSBD *modemp);

      virtual void begin();

    private:
      IridiumSBD *modem;

};

#endif
