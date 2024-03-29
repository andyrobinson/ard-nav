#include <version.h>
#include <SPI.h>
#include <Wire.h>
#include <Compass.h>
#include <MServo.h>
#include <WindSensor.h>
#include <Timer.h>
#include <Gps.h>
#include <SDLogger.h>
#include "wiring_private.h"

#define MAJOR_VERSION         111
#define RUDDER_CHANNEL      0
#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

using namespace Angle;

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

MicroMaestro maestrolib(Serial3);

MServo servo(&maestrolib);

WindSensor windsensor;
Timer timer;

// Dependency injection
Compass compass(&timer);
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass);

char buf[50];
MagResult rbearing;
uint16_t rudder_pos = 135;
uint16_t rudder_diff = 90;

void setup() {
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  servo.setSpeed(RUDDER_CHANNEL, 15);
  servo.setAccel(RUDDER_CHANNEL, 0);

  Wire.begin();  // no longer included in compass or windsensor
  compass.begin();
  gps.begin();
  logger.begin();

  sprintf(buf, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  logger.banner(buf);
}

void loop() {

  rbearing = compass.raw_bearing();
  compass.bearing(); // this adds a significant amount of time
  delay(1000);

  servo.write(RUDDER_CHANNEL, rudder_pos);

  logger.msg("compass eg");

  if (compass.err_percent() == 100) {
    logger.banner("Error limit reached, will reset");
  }

  rudder_diff = -rudder_diff;
  rudder_pos = rudder_pos + rudder_diff;
}
