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
//#include <SerialLogger.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

// Route - due North
waypoint route[] = {{"Chorlton"{53.44096, -2.27235, 10.0}},
                    {"Up North", {80.0, -2.27, 0.1}},
                    {"Dn South", {10.0, -2.27, 0.1}}};

WindSensor windsensor;
Servo sail_servo;
Servo rudder_servo;
Compass compass;
Timer timer;
Gps gps;
Globe globe;
Logger logger;

// Dependency injection
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
}

void loop() {
  selftest.test();
  captain.voyage(route,1);
  while(true){};
}
