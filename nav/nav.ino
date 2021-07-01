#include <VSServoSamd.h>
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
#include <RotaryPID.h>
#include <version.h>

#define MAJOR_VERSION 99 // for test
#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

WindSensor windsensor;
VSServoSamd sail_servo;
VSServoSamd rudder_servo;
Compass compass;
Timer timer;
Gps gps;
Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
SDLogger logger(&gps, &windsensor, &compass);
Sail sail(&sail_servo);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer, &logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
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
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  // selftest.test();
  sprintf(logmsg, "Navigating v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  sprintf(logmsg, "Switches %3d", switches.value()); logger.banner(logmsg);

  uint8_t sw = switches.value() & 3; // four routes configurable
  route journey = plattfields[sw];

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Finished Navigation :-)");

  while(true){};
}
