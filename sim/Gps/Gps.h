#ifndef GpsSim_h
#define GpsSim_h

#include "Position.h"
#include "Utility.h"
#include "Boat.h"
#include "Timer.h"

#define FIX_DGPS 2
#define MAX_ACCURACY_METRES 2
#define MIN_SPEED 0.01

using namespace Position;

typedef struct {
  position pos;
  short fix;
  float mps;
  float avg_mps;
  long unixTime;
} gpsResult;

class Gps
{
  public:
    Gps(Boat *boatp, Timer *timerp);
    void data(uint32_t max_millis, gpsResult *result);

  private:
    Boat *boat;
    Timer *timer;
    float avg_speed;
};
#endif
