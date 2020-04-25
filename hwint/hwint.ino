#include <Arduino.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
#include <Rudder.h>
#include <Sail.h>
#include <Gps.h>

// pick between loggers here
#include <DisplayLogger.h>
//#include <SerialLogger.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

using namespace Angle;
using namespace Position;

Servo rudder_servo;
Rudder rudder(&rudder_servo);
Servo sail_servo;
Rudder sail(&sail_servo);

Gps gps;
WindSensor windsensor;
Compass compass;
Logger logger(&gps, &windsensor, &compass);
gpsResult gpsReading;

void setup() {
  logger.begin();
  windsensor.begin();
  compass.begin();

  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);

  gps.begin();

  rudder_servo.attach(RUDDER_SERVO_PIN);
  sail_servo.attach(SAIL_SERVO_PIN);

  logger.banner(" Init!");

}

void move_rudder() {
  static angle rudder_position = 0;
  static short rudder_increment = 5;

  rudder_position = rudder_position + rudder_increment;
  if (abs(rudder_position) >= RUDDER_MAX_DISPLACEMENT) {
    rudder_increment = -rudder_increment;
  }
  rudder.set_position(rudder_position);
}

void wait_for_gps_fix() {
  static int gps_wait = 2000;
  gps.data(gps_wait, &gpsReading);
  if (gpsReading.fix == -1) {
    gps_wait = gps_wait + 5000;
  }
  else if (gps_wait > 2000) {
    gps_wait = gps_wait - 1000;
  }
}

extern "C" char* sbrk(int incr);
int freeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

void appendFreemem(char *msg) {
  int freemem = freeMemory();
  char intbuf[16];
  itoa(freemem, intbuf, 10);
  strcat(msg, intbuf);
}

void loop() {
  angle wind = windsensor.relative();

  move_rudder();
  sail.set_position(wind);

  wait_for_gps_fix();

  char msg[40] = "mem ";
  appendFreemem(msg);

  logger.msg(msg);

  delay(500);
}
