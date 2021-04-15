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

// Dependency injection
SerialLogger logger(&gps, &windsensor, &compass);

Sail sail(&sail_servo);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer, &logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
//  sail_servo.attach(SAIL_SERVO_PIN);
//  rudder_servo.attach(RUDDER_SERVO_PIN);

//  windsensor.begin();
  compass.begin();
  gps.begin();
  logger.begin();
  switches.begin();
}

void loop() {
  // selftest.test();
  logger.banner("Starting");

  byte sw = switches.value() & 3; // only four routes at present
  char logmsg[22];
  sprintf(logmsg, "Route %2d", sw); logger.banner(logmsg);

  waypoint *route = plattfields[sw];
  int routesize = ARRAY_SIZE(route);
  sprintf(logmsg, "Size %2d", routesize); logger.banner(logmsg);
  sprintf(logmsg, "point %s", route[0].label); logger.banner(logmsg);

  for (int i=0; i < routesize; i++) {
    sprintf(logmsg, "point %s", route[i].label); logger.banner(logmsg);
  }

//  captain.voyage(route,ARRAY_SIZE(route));
//  logger.banner("Done");

  while(true){};
}
