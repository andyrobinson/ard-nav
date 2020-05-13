#include "Arduino.h"
#include "DisplayLogger.h"
#include "Adafruit_SSD1306.h"
#include "avr/dtostrf.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_MOSI  7
#define OLED_CLK   8
#define OLED_DC    9
#define OLED_CS    13
#define OLED_RESET 11

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void messageAt(int y, char *msg) {
  display.setCursor(0,8*y);
  display.println(msg);
  display.display();
}

void append_double5pl(char *buf, double dbl) {
  if (dbl > -1000.0 && dbl < 1000.0) {
    char stringdbl[11];
    dtostrf(dbl,10,5,stringdbl);
    strcat(buf,stringdbl);
  } else {
    strcat(buf, "****.*****");
  }
}

void append_double1pl(char *buf, double dbl) {
  if (dbl > -10.0 && dbl < 10.0) {
    char stringdbl[5];
    dtostrf(dbl,4,1,stringdbl);
    strcat(buf,stringdbl);
  } else {
    strcat(buf, "**.*");
  }
}

void append_int(char *buf, int i) {
  char stringint[8]; // big enough for any int
  itoa(i,stringint, BASE10);
  strcat(buf,stringint);
}

void append_digit(char *buf, int i) {
  if (i > -10 && i < 10) {
    char stringint[4];
    itoa(i,stringint,BASE10);
    strcat(buf,stringint);
  } else {
    strcat(buf, "***");
  }
}

unsigned long last_log_time=millis();
boolean hold_last_message=false;

boolean time_to_change_message() {
   unsigned long time_since_last = millis() - last_log_time;
   boolean time_to_change = (time_since_last > 10000 ||
           (!hold_last_message && time_since_last > 2000));

    if (time_to_change) {
      last_log_time = millis();
      hold_last_message = false;
    }

    return time_to_change;
}

extern "C" char* sbrk(int incr);
int dispFreeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

Logger::Logger() {}

Logger::Logger(Gps *gpsp, WindSensor *windsensorp, Compass *compassp):
  gps(gpsp), compass(compassp), windsensor(windsensorp) {}

void Logger::begin() {
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  destination = ' ';
  tack = '0';
}

void Logger::setdest(char destletter) {
  destination = destletter;
}

void Logger::settack(char tackletter) {
  tack = tackletter;
}

void Logger::banner(char *message) {
      display.clearDisplay();
      display.setTextSize(1);
      char stars[]="********************";
      messageAt(0, stars);
      messageAt(3, stars);
      display.setTextSize(2);
      display.setCursor(0,8);
      display.println(message);
      display.display();
      hold_last_message = true;
}

void Logger::msg(char *message) {

  if (time_to_change_message()) {
      gps->data(GPS_WAIT_MILLIS, &gpsReading);
      angle wind = windsensor->relative();
      uangle bearing = compass->bearing();
      int mem=dispFreeMemory();

      display.clearDisplay();
      display.setTextSize(1);
      char buf[22];
      buf[0]='\0';
      append_double5pl(buf, gpsReading.pos.latitude);
      append_double5pl(buf, gpsReading.pos.longitude);
      messageAt(0, buf);

      sprintf(buf, "%c%c W%4d C%4d  T%4d", destination, tack, wind, bearing, gpsReading.unixTime %1000);
      messageAt(1, buf);

      buf[0]='\0';
      strcat(buf, "ms"); append_double1pl(buf, gpsReading.mps);
      strcat(buf," Fx"); append_digit(buf, gpsReading.fix);
      strcat(buf," M"); append_int(buf, mem);
      messageAt(2, buf);

      if (strlen(message) < 22) {
        sprintf(buf,"%s", message);
        messageAt(3, buf);
      } else {
        sprintf(buf,"%s","** MSG OVERFLOW **");
        messageAt(3, buf);
      }
  }
}
