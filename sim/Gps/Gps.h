#ifndef Gps_h
#define Gps_h

#include "Position.h"
#include "Utility.h"
#include "Boat.h"

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
} gpsResult;

class Gps
{
  public:
    Gps(Boat *boatp);
    void data(uint32_t max_millis, gpsResult *result);

  private:
    float avg_speed;

#endif
