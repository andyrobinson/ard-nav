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
  static char displaybuff[4][41] = {"                                        ",
                                    "                                        ",
                                    "                                        ",
                                    "                                        "};

  for (int i = 0; i <=40; i++) {
    displaybuff[y][i]=msg[i];
  }

  display.clearDisplay();

  for (int j = 0; j < 4; j++) {
    display.setCursor(0,8*j);
    display.println(displaybuff[j]);
  }
  display.display();
}

void append_double(char *buf, double dbl, int places) {
  char stringdbl[9];
  dtostrf(dbl,9,places,stringdbl);
  strcat(buf,stringdbl);
}

void append_int(char *buf, int i) {
  char stringint[8];
  itoa(i,stringint,7);
  strcat(buf,stringint);
}

Logger::Logger() {}

void Logger::begin() {
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
}

void Logger::info(gpsResult *gps_result, angle wind, uangle bearing, char *message) {
  display.clearDisplay();
  static char buf[41];

  // buf[0]=0; // empty string
  // append_double(buf, gps_result->pos.latitude,5);
  // append_double(buf, gps_result->pos.longitude,5);
  // messageAt(0, buf);

  sprintf(buf, "W%4d C%4d  T%4d", wind, bearing,gps_result->unixTime %1000);
  messageAt(1, buf);

  // buf[0]=0; // empty string
  // strcat(buf, "mps");  append_double(buf, gps_result->mps,1);
  // strcat(buf," Fx ");  append_int(buf, gps_result->fix);
  // messageAt(2, buf);

  msg(message);
}

void Logger::msg(char *message) {
  static char buf[41];
  if (strlen(message) < 40) {
    sprintf(buf,"%s", message);
    messageAt(3, buf);
  } else {
    sprintf(buf,"%s","** MSG OVERFLOW **");
    messageAt(3, buf);
  }
}
