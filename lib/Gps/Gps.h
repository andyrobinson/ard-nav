#ifndef Gps_h
#define Gps_h

#include "Arduino.h"
#include "Position.h"
#include "Utility.h"
#include "Timer.h"

#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10

#define INTERRUPT_SAMPLE_RATE_MICRO_SECS 800

#define FIX_UNPARSEABLE_DATA   -2
#define FIX_NONE               -1
#define FIX_INVALID             0
#define FIX_GPS                 1
#define FIX_DGPS                2

#define KNOTS_TO_METRES_PER_SEC 0.514444
#define MAX_ACCURACY_METRES 2
#define MAX_POSSIBLE_SPEED 10.0 // acutally it's more like 1.6 m/s
#define MIN_SPEED 0.01 // to avoid possible divide by zero errors

using namespace Position;

typedef struct {
  position pos;
  short fix;
  float mps;
  float avg_mps;
  long unixTime;
  int32_t fpLatitude;
  int32_t fpLongitude;
} gpsResult;

class Gps
{
  public:
    Gps(Timer *timerp);
    void begin();
    void data(uint32_t max_millis, gpsResult *result);
    void tcStartCounter();
    void tcReset();
    void tcDisable();

private:
  void tcConfigure(int sampleRate);
  bool tcIsSyncing();
  Timer *timer;
};

#endif
