#include "Arduino.h"
#include "Logger.h"
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

void messageAt(int y, String msg) {
  static String displaybuff[4]={"","","",""};

  displaybuff[y] = msg;
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

void Logger::info(position *current_position, angle wind, uangle bearing, String message) {
  static char buf[21];

  sprintf(buf, "%10.5d %10.5d", current_position->latitude, current_position->longitude);
  messageAt(0, buf);

  sprintf(buf, "W%4d C%4d         ", wind, bearing);
  messageAt(1, buf);

  for (int i = 0; i <= 20; i++) {
    buf[i] = message[i]
  }
  messageAt(2, buf);
}
