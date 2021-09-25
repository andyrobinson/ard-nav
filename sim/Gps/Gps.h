#ifndef Gps_h
#define Gps_h

#include "Position.h"
#include "Utility.h"
#include "Boat.h"

#define FIX_DGPS                2

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
    Gps(Boat *boatp);
    void data(uint32_t max_millis, gpsResult *result);

  private:
    float avg_speed;

#endif
