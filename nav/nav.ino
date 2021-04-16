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
#include "avr/dtostrf.h" // remove me after the hacking about

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
  char buff[11];
  sprintf(logmsg, "Route %2d", sw); logger.banner(logmsg);

  route journey = plattfields[sw];
  sprintf(logmsg, "Size %2d", journey.length); logger.banner(logmsg);

  for (int i=0; i < journey.length; i++) {
    waypoint wp = journey.waypoints[i];
    dtostrf(wp.pos.latitude, 10, 5, buff);
    sprintf(logmsg, "point %s %s", wp.label, buff); logger.banner(logmsg);
  }

//  captain.voyage(journey.waypoints, journey.length);
//  logger.banner("Done");

  while(true){};
}
