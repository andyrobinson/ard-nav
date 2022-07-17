#include <Position.h>
#include <I2C.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <TankHelm.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <Utility.h>
#include <Routes.h>
#include <RotaryPID.h>
#include <Switches.h>
#include <SDLogger.h>
//#include <SerialLogger.h>
#include <Battery.h>
#include <version.h>
#include <MServo.h>

#define MAJOR_VERSION 2 // for test

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

char logmsg[22];

// Dependency injection

WindSensor windsensor;
Timer timer;
Globe globe;
Switches switches;
I2C i2c;
Battery battery(&analogRead);

MServo servo_control;

Compass compass(&i2c, &timer);
Gps gps(&timer);

SDLogger logger(&gps, &windsensor, &compass, &battery, 3000);
//SerialLogger logger(&gps, &windsensor, &compass, &battery);

Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo_control);
TankHelm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
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

  i2c.begin();
  servo_control.begin();
  rudder.begin();
  sail.begin();
  compass.begin();
  gps.begin();
  switches.begin();
  logger.begin();
  timer.wait(5000); // don't do anything, give it all a chance to settle
}

void loop() {

  sprintf(logmsg, "Tank test v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
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
