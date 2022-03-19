#include <version.h>
#include <wiring_private.h>

#include <SPI.h>
#include <Wire.h>
#include <Compass.h>

#define MAJOR_VERSION         111
#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Compass compass;
char buf[50];
MagResult rbearing;

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

void setup() {
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  compass.begin();
  sprintf(buf, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  Serial.println(buf);
}

void loop() {

  int rudder = 25;

  // lots of exercise for I2C
  for (int i=0; i< 110;i++) {
    rbearing = compass.raw_bearing();
    compass.bearing(); // this adds a significant amount of time
    delay(5);
  }

  Serial3.write(rudder);

  Serial.print(millis()/1000); Serial.print(",");
  sprintf(buf, "B: %d", compass.bearing());
  Serial.print(buf);Serial.print(",(");

  Serial.print(rbearing.x); Serial.print(",");
  Serial.print(rbearing.y); Serial.print(",");
  Serial.print(rbearing.z); Serial.println(")");

  rudder = -rudder;
}
