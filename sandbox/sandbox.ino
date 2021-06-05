// Adafruit SSD1306 - Version: Latest
#include <SPI.h>
#include <Wire.h>
#include <WindSensor.h>
#include <Compass.h>
#include <math.h>
#include <Angle.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Rudder.h>
#include <Switches.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
// #define OLED_MOSI   9
// #define OLED_CLK   10
// #define OLED_DC    11
// #define OLED_CS    12
// #define OLED_RESET 13

#define OLED_MOSI  7
#define OLED_CLK   8
#define OLED_DC    9
#define OLED_CS    13
#define OLED_RESET 11
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

// Instantiate the Serial2 class
Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM1_Handler()    // Interrupt handler for SERCOM1
{
  Serial2.IrqHandler();
}

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#define PMTK_Q_RELEASE "$PMTK605*31"

#define RUDDER_SERVO_PIN 5

using namespace Angle;
WindSensor windsensor;
Compass compass;
Switches switches;
Servo rudder_servo;
Rudder rudder(&rudder_servo);
float rc_percent;

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  rudder_servo.attach(RUDDER_SERVO_PIN);
  switches.begin();

  // if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }

  // Serial2.begin(9600);
  // delay(2000);

  // Serial.println("Get version!");
  // Serial2.println(PMTK_Q_RELEASE);

  // you can send various commands to get it started
  //mySerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Serial2.println(PMTK_SET_NMEA_OUTPUT_ALLDATA);

  // Serial2.println(PMTK_SET_NMEA_UPDATE_1HZ);
   // windsensor.begin();
   // compass.begin();

  // init display
  // display.clearDisplay();
  // display.setTextSize(1);      // Normal 1:1 pixel scale
  // display.setTextColor(WHITE); // Draw white text
}

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


void loop() {

  // if (Serial2.available()) {
  //   //byte byteRead = Serial2.read();
  //   char c = Serial2.read();
  //   Serial.write(c);
  // }

  // char buf[20];
  //
  // angle wind = windsensor.relative();
  // uangle heading = compass.bearing();
  //
  // sprintf(buf, "W%4d C%4d", wind, heading);
  // Serial.println(buf);
  //
  // messageAt(0,buf);

  // byte sw = switches.value();
  // short rudder_pos = (12 * sw) - 45;
  // Serial.print("Reading: ");
  // Serial.println(sw);
  //
  // rudder.set_position(rudder_pos);
  rc_percent = switches.dial_percent();
  Serial.println(rc_percent);

  delay(1000);
}
