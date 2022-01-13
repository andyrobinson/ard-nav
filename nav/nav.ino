#include <MServo.h>
#include <Position.h>
#include <Compass.h>
#include <WindSensor.h>
//#include <CompassDmac.h>
//#include <WindSensorDmac.h>
#include <CompassWire.h>
#include <WindSensorWire.h>
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

WindSensorWire windsensor;
CompassWire compass;
//CompassDmac compass;
//WindSensorDmac windsensor;
MServo servo_control;
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

    GCLK->GENDIV.reg = GCLK_GENDIV_DIV(12) |          // Divide the 48MHz clock source by divisor 12: 48MHz/12=4MHz
                       GCLK_GENDIV_ID(3);             // Select Generic Clock (GCLK) 3
    while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

    GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |            // Set the duty cycle to 50/50 HIGH/LOW
                        GCLK_GENCTRL_GENEN |          // Enable GCLK3
                        GCLK_GENCTRL_SRC_DFLL48M |    // Set the 48MHz clock source
                        GCLK_GENCTRL_ID(3);           // Select GCLK3
    while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization

    Wire.begin();                                     // Set-up the I2C port
    sercom3.disableWIRE();                            // Disable the I2C SERCOM
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |          // Enable GCLK3 as clock soruce to SERCOM3
                        GCLK_CLKCTRL_GEN_GCLK3 |      // Select GCLK3
                        GCLK_CLKCTRL_ID_SERCOM3_CORE; // Feed the GCLK3 to SERCOM3
    while (GCLK->STATUS.bit.SYNCBUSY);                // Wait for synchronization
    SERCOM3->I2CM.BAUD.bit.BAUD = 4000000 / (2 * 10000) - 1;   // Set the I2C clock rate to 10kHz
    sercom3.enableWIRE();                             // Enable the I2C SERCOM

  servo_control.begin();
  delay(1000);
  rudder.begin();
  sail.begin();
  compass.begin();
//  windsensor.begin();
//  delay(100); // to allow time for I2C
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
