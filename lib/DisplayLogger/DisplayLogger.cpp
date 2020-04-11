#include "Arduino.h"
#include "DisplayLogger.h"
#include "Adafruit_SSD1306.h"

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
  static char displaybuff[4][41]={"                                        ",
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
  for (int i=0; i<=40; i++) {
    buf[i]=' ';
  }

  sprintf(buf, "%10.5f %10.5f", gps_result->pos.latitude, gps_result->pos.longitude);
  messageAt(0, buf);

  sprintf(buf, "W%4d C%4d         ", wind, bearing);
  messageAt(1, buf);

  sprintf(buf, "mps%5.2f Fx%3d      ", gps_result->mps, gps_result->fix);
  messageAt(2, buf);

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
