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

  //sercom3.setTimeoutInMicrosWIRE(25000ul, false);  // for new timeout

//   servo_control.begin();
//   delay(1000);
//   rudder.begin();
//   sail.begin();
  //windsensor.begin();  // this is NOT nice - the WindSensor begin must come first or not at all - I think we should make Wire.begin work only once ...
  delay(50);
  compass.begin();
// //  delay(100); // to allow time for I2C
//   gps.begin();
  logger.begin();
  // switches.begin();
}

void loop() {
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  // selftest.test();
  sprintf(logmsg, "Navigating v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

  for (int i=0;i<10;i++) {
    logger.banner("Logging ...");
    delay(1000);
  }

  // //int countdownMS = Watchdog.enable(4000);
  // //sprintf(logmsg, "Watchdog at %3d", countdownMS); logger.banner(logmsg);
  // sprintf(logmsg, "Watchdog disabled"); logger.banner(logmsg);
  //
  // uint8_t sw = switches.value() & 3; // four routes configurable
  // route journey = plattfields[sw];
  //
  // // a little indicator that we're starting
  // rudder.set_position(-45);
  // sail.set_position(-45);
  // timer.wait(4000);
  // sail.set_position(45);
  // rudder.set_position(45);
  // timer.wait(4000);
  //
  // captain.voyage(journey.waypoints, journey.length);
  logger.banner("Finished Navigation :-)");

  while(true){};
}
