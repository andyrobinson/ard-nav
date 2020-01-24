#include "Gps.h"
#include "Adafruit_GPS.h"

Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

Adafruit_GPS AGPS(&Serial2);

void Gps::begin() {
  AGPS.begin(9600);
  AGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);  // minimum
  AGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
}

gpsResult Gps::position(uint32_t max_millis) {
  uint32_t timer = millis();
  gpsResult result;

  AGPS.wakeup();

  do {
    if (AGPS.newNMEAreceived() && !AGPS.parse(AGPS.lastNMEA())) {
      continue;
    }

    result.unix_time = unixTime(AGPS.year, AGPS.month, AGPS.day, AGPS.hour, AGPS.minute, AGPS.seconds);
    result.fix = FIX_NONE;
    result.speed = 0.0;

    if (AGPS.fix) {
      result.latitude = AGPS.latitudeDegrees;
      result.longitude = AGPS.longitudeDegress;
      result.error = AGPS.PDOP;
      result.knots = AGPS.speed;
      result.fix = AGPS.fixquality;
    }
  } while (!AGPS.fix && ((millis() - timer) < max_millis))

  AGPS.standby();

  return result;
}

// still not quite right, we add the leap day at the 1st Jan, then take it off again at 1st March
long Gps::unix_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t second) {
  long years_since_1970 = year - 1970;
  long leap_years = (years_since_1970 + 2) / 4;
  long year_days = (years_since_1970) * 365 + (leap_years);
  long month_days = 0;

  switch (month-1) {
    case 11: month_days = month_days + 30;
    case 10: month_days = month_days + 31;
    case 9: month_days = month_days + 30;
    case 8: month_days = month_days + 31;
    case 7: month_days = month_days + 31;
    case 6: month_days = month_days + 30;
    case 5: month_days = month_days + 31;
    case 4: month_days = month_days + 30;
    case 3: month_days = month_days + 31;
    case 2: month_days = month_days + 28;
    case 1: month_days = month_days + 31;
  }

  return ((year_days + month_days + day) * 86400) + (hour * 3600) + (min * 60) + second;
}
