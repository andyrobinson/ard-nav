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
#include <Utility.h>
#include <Routes.h>
#include <RotaryPID.h>
#include <Switches.h>
#include <SDLogger.h>
// #include <SerialLogger.h>
#include <Battery.h>
#include <version.h>
#include <MServo.h>
#include <IridiumSBD.h>
#include <SatComm.h>

#define MAJOR_VERSION 3 // first successful reliable navigation
#define STARTUP_WAIT_FOR_FIX_MS 60000
#define STARTUP_ATTEMPTS 10

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

char logmsg[40];

#define IRIDIUM_SERIAL Serial1
#define IRIDIUM_SLEEP_PIN 6

// Dependency injection

Timer timer;
Globe globe;
Switches switches;
I2C i2c;
MServo servo_control;

Battery battery(&analogRead, &timer);
WindSensor windsensor(&i2c);
Compass compass(&i2c, &timer);
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass, &battery, &switches, &timer, 0);
// SerialLogger logger(&gps, &windsensor, &compass, &battery);
IridiumSBD modem(IRIDIUM_SERIAL, IRIDIUM_SLEEP_PIN);
SatComm satcomm(&modem, &timer, &gps, &battery, &compass, &logger);
Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches);
Rudder rudder(&servo_control);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &satcomm, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &satcomm, &logger);
Captain captain(&navigator);

//Iridium callback, used when no-block waiting
bool ISBDCallback() {
    return helm.steer_and_continue();
}

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

  logger.begin();
  IRIDIUM_SERIAL.begin(19200);
  satcomm.begin();
  i2c.begin();
  servo_control.begin();
  rudder.begin();
  sail.begin();
  compass.begin();
  gps.begin();
  switches.begin();
  timer.wait(5000); // don't do anything, give it all a chance to settle
}

void loop() {

  sprintf(logmsg, "System boot v%2d.%2d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

  // a little indicator that we're starting
  sail.set_position(0);
  gpsResult gps_data = {{0.0, 0.0, 0.0}, FIX_NONE, 0.0, 0, 0, 0, 0};

  // try and get a GPS fix before logging so that it goes in the same file
  short remaining_attempts = STARTUP_ATTEMPTS;
  while (gps_data.fix <= FIX_NONE and remaining_attempts-- > 0) {
    rudder.set_position(RUDDER_MAX_DISPLACEMENT);
    gps.data(STARTUP_WAIT_FOR_FIX_MS, &gps_data);
    rudder.set_position(0);
    timer.wait(3000);
  }

  // and we're off

  uint8_t sw = switches.value(); // eight routes configurable
  route journey = plattfields[sw];

  logger.write_version(MAJOR_VERSION,MINOR_VERSION);
  sprintf(logmsg, "Starting v%2d.%2d: route [%1d]", MAJOR_VERSION, MINOR_VERSION, sw); logger.banner(logmsg);
  sprintf(logmsg, "Watchdog disabled"); logger.banner(logmsg);

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Finished Navigation :-)");

  while(true){};
}
