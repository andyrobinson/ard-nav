#include <MServo.h>
#include <Position.h>
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
#include <Switches.h>
#include <Routes.h>
#include <version.h>

#define MAJOR_VERSION 99 // for test

WindSensor windsensor;
MServo servo_control;
//Compass compass;
Timer timer;
Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
Gps gps(&timer);
SDLogger logger(&gps, &windsensor);
Sail sail(&servo_control);
Rudder rudder(&servo_control);
Helm helm(&rudder, &timer, &windsensor, &sail, &logger);
Tacker tacker(&helm, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
  servo_control.begin();
  delay(1000);
  rudder.begin();
  sail.begin();
  windsensor.begin();
  gps.begin();
  logger.begin();
  switches.begin();
}

void loop() {
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  sprintf(logmsg, "Navigating v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
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
