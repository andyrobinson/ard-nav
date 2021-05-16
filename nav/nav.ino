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
#include <RotaryPID.h>
#include <version.h>
#include <avr/dtostrf.h>

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
float kp,ki,kd;

Switches switches;
char logmsg[22];

// Dependency injection
SDLogger logger(&gps, &windsensor, &compass);
Sail sail(&sail_servo);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT);
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

// Steering PID parameters
  uint8_t sw2 = (switches.value() & 2) >> 1;
  uint8_t sw3 = (switches.value() & 4) >> 2;
  kp = KP * (1 + sw2);
  ki = KI * (1 + sw3);
  kd = ki/8;

  rotaryPID.set_constants(kp, ki, kd);
}

void append_float1pl(char *buf, float f) {
  if (f > -10.0 && f < 10.0) {
    char stringdbl[5];
    dtostrf(f,4,1,stringdbl);
    strcat(buf,stringdbl);
  } else {
    strcat(buf, "**.*");
  }
}

void loop() {
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  selftest.test();
  sprintf(logmsg, "Navigating v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  sprintf(logmsg, "Switches %3d", switches.value()); logger.banner(logmsg);
  sprintf(logmsg, "K ", switches.value());
  append_float1pl(logmsg,kp);append_float1pl(logmsg,ki);append_float1pl(logmsg,kd);
  logger.banner(logmsg);

  uint8_t sw = switches.value() & 1; // only two routes configurable
  route journey = plattfields[sw];

  captain.voyage(journey.waypoints, journey.length);
  logger.banner("Done");

  while(true){};
}
