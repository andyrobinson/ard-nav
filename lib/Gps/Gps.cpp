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

gpsResult Gps::data(uint32_t max_millis) {
  uint32_t timer = millis();
  gpsResult result;

  AGPS.wakeup();

  do {
    AGPS.read();

    if (AGPS.newNMEAreceived() && !AGPS.parse(AGPS.lastNMEA())) {
      continue;
    }

    result.unixTime = 0;
    result.fix = FIX_NONE;
    result.knots = 0.0;
    result.unixTime = unix_time(AGPS.year, AGPS.month, AGPS.day, AGPS.hour, AGPS.minute, AGPS.seconds);

    if (AGPS.fix) {
      result.pos.latitude = AGPS.latitudeDegrees;
      result.pos.longitude = AGPS.longitudeDegrees;
      result.pos.error = AGPS.PDOP;
      result.knots = AGPS.speed;
      result.fix = AGPS.fixquality;
    }
  } while (!AGPS.fix && ((millis() - timer) < max_millis));

  AGPS.standby();

  return result;
}
