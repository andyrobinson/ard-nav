#include "Gps.h"
#include "Adafruit_GPS.h"

using namespace Utility;

Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

Adafruit_GPS AGPS(&Serial2);

Gps::Gps() {}

void Gps::begin() {
  AGPS.begin(9600);
  AGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);  // with fix quality and satellites
  AGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
}

void Gps::data(uint32_t max_millis, gpsResult *result) {
  uint32_t timer = millis();

  AGPS.wakeup();

  do {
    AGPS.read();

    if (AGPS.newNMEAreceived() && !AGPS.parse(AGPS.lastNMEA())) {
      continue;
    }

    result->fix = FIX_NONE;
    result->mps = 0.0;
    result->unixTime = unix_time(AGPS.year, AGPS.month, AGPS.day, AGPS.hour, AGPS.minute, AGPS.seconds);

    if (AGPS.fix) {
      result->pos.latitude = AGPS.latitudeDegrees;
      result->pos.longitude = AGPS.longitudeDegrees;
      result->pos.error = AGPS.PDOP * MAX_ACCURACY_METRES;
      result->mps = AGPS.speed * KNOTS_TO_METRES_PER_SEC;
      result->fix = AGPS.fixquality;
    }
  } while (!AGPS.fix && ((millis() - timer) < max_millis));

  AGPS.standby();
}
