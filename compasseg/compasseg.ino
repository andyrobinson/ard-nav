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
Compass compass;
Timer timer;

// Dependency injection
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

  // while (!Serial); // wait for Serial to be ready
  // Serial.begin(19200);

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

  // lots of exercise for I2C
  for (int i=0; i< 110;i++) {
    rbearing = compass.raw_bearing();
    compass.bearing(); // this adds a significant amount of time
    delay(5);
  }

  servo.write(RUDDER_CHANNEL, rudder_pos);

  long start = millis();
  Serial.print(millis()/1000); Serial.print(",");
  sprintf(buf, "B: %d", compass.bearing());
  Serial.print(buf);Serial.print(",");
  sprintf(buf, "E: %d", compass.err_percent());
  Serial.print(buf);Serial.print(",(");

  Serial.print(rbearing.x); Serial.print(",");
  Serial.print(rbearing.y); Serial.print(",");
  Serial.print(rbearing.z); Serial.print(")");
  Serial.println(millis()-start);

  logger.msg("compass eg");
  if (compass.err_percent() == 100) {
    logger.banner("I2C Failed!");
    Serial.println("*** I2C Failed ***");
    while(true){};
  }

  rudder_diff = -rudder_diff;
  rudder_pos = rudder_pos + rudder_diff;
}
