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
#include <DisplayLogger.h>
#include <SerialLogger.h>
#include <SDLogger.h>
#include <MultiLogger.h>
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

// Dependency injection
SDLogger sdlogger(&gps, &windsensor, &compass);
DisplayLogger displaylogger(&gps, &windsensor, &compass);
SerialLogger seriallogger(&gps, &windsensor, &compass);
Logger* loggers[] = {&sdlogger};
MultiLogger logger(&gps, &windsensor, &compass, loggers, 1);

Sail sail(&sail_servo);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer, &logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &logger);
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

  byte sw = switches.value() & 3; // only four routes at present
  waypoint route[] = plattfields[sw];

  char logmsg[22];
  sprintf(logmsg, "Route %2d", sw); logger->banner(logmsg);

  captain.voyage(route,ARRAY_SIZE(route));
  logger.banner("Done");

  while(true){};
}
