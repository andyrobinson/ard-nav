#include <Servo.h>
#include <Position.h>
#include <Compass.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <SelfTest.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <SDLogger.h>
#include <SerialLogger.h>
#include <Utility.h>
#include <Routes.h>
#include <Switches.h>
#include <version.h>

#define MAJOR_VERSION 1
#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

WindSensor windsensor;
Servo sail_servo;
Servo rudder_servo;
Compass compass;
Timer timer;
Gps gps;
Globe globe;
Switches switches;
char logmsg[22];

// Dependency injection
SDLogger logger(&gps, &windsensor, &compass);

Sail sail(&sail_servo);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer, &logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &switches, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
  sail_servo.attach(SAIL_SERVO_PIN);
  rudder_servo.attach(RUDDER_SERVO_PIN);

  windsensor.begin();
  compass.begin();
  gps.begin();
  logger.begin();
  switches.begin();
}

void loop() {
  selftest.test();
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

  byte sw = switches.value() & 1; // only two routes configurable
  route journey = plattfields[sw];

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Done");

  while(true){};
}
