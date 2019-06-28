// Adafruit SSD1306 - Version: Latest 
#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  Wire.begin();

}

void loop() {

  char buff[10]; 

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  messageAt(1,"Starting ...");

  uint16_t angle = getAngle();
  sprintf (buff,"%d",angle);
  messageAt(2,String("  Angle: ") + buff);
  delay(1000);    
  
}

uint16_t getAngle() {
  byte endTransResult;
  uint16_t result = 0;
  const uint8_t registerAddress = 0xFE;
  const uint8_t deviceAddress = 0x40;

  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);

  endTransResult = Wire.endTransmission(false);

  if (endTransResult){
    Serial.println("I2C error: " + String(endTransResult));
  }


  Wire.requestFrom(deviceAddress, (uint8_t) 2);
  byte upper8bits = Wire.read();
  byte lower6bits = Wire.read();

  result = (((uint16_t) upper8bits) << 6) + (lower6bits & 0x3F);

  
  Serial.println(upper8bits);
  Serial.println(lower6bits);
  Serial.println(result);
  return result;

}

String displaybuff[4]={"","","",""};

void messageAt(int y, String msg) {
  
  displaybuff[y] = msg;
  display.clearDisplay();

  for (int j = 0; j < 4; j++) {
    display.setCursor(0,8*j);           
    display.println(displaybuff[j]); 
  }
  display.display();
}

void displayMessage(String msg) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(msg);

  display.display();
  delay(2000);

}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}
