#ifndef Gps_h
#define Gps_h

#include "Arduino.h"
#include "Position.h"
#include "Utility.h"

#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

#define FIX_NONE -1
#define FIX_INVALID 0
#define FIX_GPS 1
#define FIX_DGPS 2

using namespace Position;

typedef struct {
  position pos;
  short fix;
  float knots;
  long unixTime;
} gpsResult;

class Gps
{
  public:
    Gps();
    void begin();
    gpsResult data(uint32_t max_millis);
};

#endif
