#include "Gps.h"

using namespace Utility;

// class based stuff
Gps::Gps(Boat *boatp): boat(boatp), avg_speed(MIN_SPEED) {}

void Gps::data(uint32_t max_millis, gpsResult *result) {

    if (AGPS.fix) {
      result->pos.latitude = ;
      result->pos.longitude = AGPS.longitudeDegrees;
      result->pos.error = AGPS.PDOP * MAX_ACCURACY_METRES;

      result->mps = max1(MIN_SPEED, min1(AGPS.speed * KNOTS_TO_METRES_PER_SEC, MAX_POSSIBLE_SPEED));
      avg_speed = (0.9 * avg_speed) + (0.1 * result-> mps); // 10 point moving average
      result->avg_mps = avg_speed;

      result->fix = FIX_GPS;
    }

    AGPS.pause(false);


}
