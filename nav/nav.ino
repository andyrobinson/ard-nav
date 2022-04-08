#include <MServo.h>
#include <Position.h>
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
#include <version.h>

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

MicroMaestro maestrolib(Serial3);
MServo servo_control(&maestrolib);

WindSensor windsensor;
Compass compass;
Timer timer;
Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass);
Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo_control);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
  // must come first
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  rudder.begin();
  sail.begin();
  Wire.begin();   // no longer included in compass or windsensor
  compass.begin();
  gps.begin();
  logger.begin();
  switches.begin();

}

void loop() {
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  // selftest.test();
  sprintf(logmsg, "Navigating v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

  //int countdownMS = Watchdog.enable(4000);
  //sprintf(logmsg, "Watchdog at %3d", countdownMS); logger.banner(logmsg);
  sprintf(logmsg, "Watchdog disabled"); logger.banner(logmsg);

  uint8_t sw = switches.value() & 3; // four routes configurable
  route journey = plattfields[sw];

  // a little indicator that we're starting
  rudder.set_position(-45);
  sail.set_position(-45);
  timer.wait(4000);
  sail.set_position(45);
  rudder.set_position(45);
  timer.wait(4000);

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Finished Navigation :-)");

  while(true){};
}
