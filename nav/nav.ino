#include <MServo.h>
#include <Position.h>
#include <I2C.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <SDLogger.h>
#include <Utility.h>
#include <Routes.h>
#include <RotaryPID.h>
#include <Switches.h>
#include <Battery.h>
#include <version.h>
#include "wiring_private.h"

#define MAJOR_VERSION 99 // for test

/*

This is a good guide to configuring the SERCOMS on SAM21 Arduino

https://cdn-learn.adafruit.com/downloads/pdf/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports.pdf

Serial ports used Sercom - not sure if this is the cause of the problems, but the definitions
are certainly used more than once, and have different values

+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
|  Uart   | Sercom | Pin Tx | Pin Rx | Pad Tx | Pad Rx | Board Tx | Board Rx | Use       |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
| Serial  |    5   |   22   |  23    |    2   |   3    |   D30    |   D31    | EDBG      |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
| Serial1 |    0   |   10   |  11    |    2   |    3   |   D1     |   D0     | Sat       |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
| Serial2 |    1   |   36   |  34    |    2   |    3   |   D10    |   D12    | GPS       |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
| Serial3 |    2   |   2    |   3A   |    2   |    1   |   D2     |   D3     | Servo     |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
| (None)  |    3   |   20   |   21   |    0   |    1   |   SDA    |   SCL    | I2C(Wire) |
|         |        |        |        |        |        |  (D20)   |  (D21)   |           |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+
|         |    4   |   2    |   3    |    1   |    2   |   D      |          | SPI       |
+---------+--------+--------+--------+--------+--------+----------+----------+-----------+

A = SERCOM ALT

0 - Used for serial text output to the serial monitor, destined for the satellite comms
2 - Servo controller  5 and 6???
EDBG
*/


#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}


char logmsg[22];

// Dependency injection

WindSensor windsensor;
Timer timer;
Globe globe;
Switches switches;
I2C i2c;
Battery battery(&analogRead);

MicroMaestro maestrolib(Serial3);
MServo servo_control(&maestrolib);

Compass compass(&i2c, &timer);
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass, &battery);
Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo_control);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
  /*  BROWN OUT CONFIGURATION
   *  Thanks to Stargirl Flowers https://blog.thea.codes/sam-d21-brown-out-detector/
   */

  // Disable the brown-out detector during configuration
  SYSCTRL->BOD33.bit.ENABLE = 0;
  while (!SYSCTRL->PCLKSR.bit.B33SRDY) {};

  // Configure the brown-out detection
  SYSCTRL->BOD33.reg = (
      // This sets the minimum voltage level to 3.0v - 3.2v.   See datasheet table 37-21.
      SYSCTRL_BOD33_LEVEL(48) |
      // reset on brownout
      SYSCTRL_BOD33_ACTION_RESET |
      // Enable hysteresis to better deal with noisy powersupplies and voltage transients.
      SYSCTRL_BOD33_HYST);

  // Enable the brown-out detector and then wait for the voltage level to settle.
  SYSCTRL->BOD33.bit.ENABLE = 1;
  while (!SYSCTRL->PCLKSR.bit.BOD33RDY) {}

  // must come first
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  rudder.begin();
  sail.begin();
  i2c.begin();
  compass.begin();
  gps.begin();
  logger.begin();
  switches.begin();

  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);

  timer.wait(5000); // don't do anything, give it all a chance to settle
}

void loop() {

  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  // selftest.test();

  //int countdownMS = Watchdog.enable(4000);
  //sprintf(logmsg, "Watchdog at %3d", countdownMS); logger.banner(logmsg);
  sprintf(logmsg, "Watchdog disabled"); logger.banner(logmsg);

  uint8_t sw = switches.value() & 3; // four routes configurable
  route journey = plattfields[sw];

  // a little indicator that we're starting
  rudder.set_position(-45);
  timer.wait(4000);
  sail.set_position(0);
  timer.wait(4000);

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Finished Navigation :-)");

  while(true){};
}
