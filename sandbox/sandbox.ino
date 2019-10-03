// Adafruit SSD1306 - Version: Latest
#include <SPI.h>
#include <Wire.h>
#include <WindSensor.h>
#include <Compass.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>
#include <Servo.h>

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

WindSensor windsensor;
Compass compass;
Servo servo;  
int servoposition = 0; 
int servoinc = 1;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  windsensor.begin();
  compass.begin();

  servo.attach(4);

}

void loop() {

  char anglebuff[20];

  char bearingbuff[20];
  char accelbuff[20];

  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text

  int angle = windsensor.angle();
  MagResult bearing = compass.bearing();
  MagResult accel = compass.accel();

  int heading = (360 + round(57.2958 * atan2((double) bearing.y, (double) bearing.x))) % 360;

  double roll = atan2((double)accel.y, (double)accel.z);
  double pitch = atan2((double) -accel.x, (double) accel.z); // reversing x accel makes it work 
  double sin_roll = sin(roll);
  double cos_roll = cos(roll);
  double cos_pitch = cos(pitch);
  double sin_pitch = sin(pitch);

  double x_final = ((double) bearing.x) * cos_pitch + ((double) bearing.y)*sin_roll*sin_pitch+((double) bearing.z)*cos_roll*sin_pitch;
  double y_final = ((double) bearing.y)*cos_roll-((double) bearing.z) * sin_roll;
  int tiltadjust = (360 + round(57.2958 * (atan2(y_final,x_final)))) % 360;
  
  sprintf (anglebuff,"Wind: %d",angle);
  sprintf (bearingbuff,"Comp: %d Tilt: %d",heading,tiltadjust);
  sprintf (accelbuff,"Accel: %d %d %d",accel.x,accel.y, accel.z);

  messageAt(1,anglebuff);
  messageAt(2,bearingbuff);
  messageAt(3,accelbuff);

  servoposition = servoposition + servoinc;

  if (servoposition % 5==0) {
    servo.write(servoposition);
  }

  if (servoposition <= 0 || servoposition >= 180) {
    servoinc = -servoinc;
  }
  
  delay(100);

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
