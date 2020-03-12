#ifndef GpsStub_h
#define GpsStub_h

#include "Position.h"

using namespace Position;

typedef struct {
  position pos;
  short fix;
  float mps;
  long unixTime;
} gpsResult;

class Gps
{
  public:
    Gps();
    void begin();
    gpsResult data(uint32_t max_millis);
    void set_data(gpsResult *new_data, int size);

  private:
    gpsResult *gps_data;
    int list_size;
    int current_position;
};

#endif
