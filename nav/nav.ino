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

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

// Route - due North
position route[] = {{80.0, -2.27, 0.1}};

WindSensor windsensor;
Servo sail_servo;
Servo rudder_servo;
Compass compass;
Timer timer;
Gps gps;
Globe globe;

// Dependency injection
Sail sail(&sail_servo);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail);
Tacker tacker(&helm, &compass, &windsensor);
Navigator navigator(&tacker, &gps, &globe);
Captain captain(&navigator);

void setup() {
  sail_servo.attach(SAIL_SERVO_PIN);
  sail_servo.attach(RUDDER_SERVO_PIN);

  windsensor.begin();
  compass.begin();
  gps.begin();
}

void loop() {
  selftest.test();
  captain.voyage(route,1);
  while(true){};
}
