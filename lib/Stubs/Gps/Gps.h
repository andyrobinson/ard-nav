#ifndef GpsStub_h
#define GpsStub_h

#include "Position.h"

using namespace Position;

#define FIX_NONE               -1

typedef struct {
  position pos;
  short fix;
  float mps;
  float avg_mps;
  unsigned short cog;
  long unixTime;
  int32_t fpLatitude;
  int32_t fpLongitude;
} gpsResult;

class Gps
{
  public:
    Gps();
    void begin();
    void data(uint32_t max_millis, gpsResult *result);
    void set_data(gpsResult *new_data, int size);

  private:
    gpsResult *gps_data;
    int list_size;
    int current_position;
};

#endif
