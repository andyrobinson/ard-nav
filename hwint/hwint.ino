#include <Arduino.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
#include <Rudder.h>
#include <Sail.h>
#include <Gps.h>

// pick between loggers here
//#include <DisplayLogger.h>
#include <SerialLogger.h>

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
Logger logger;
position current_position;
int gps_wait = 2000;
long gps_time_to_read;

void setup() {
  gps.begin();
  logger.begin();
  windsensor.begin();
  compass.begin();
  rudder_servo.attach(RUDDER_SERVO_PIN);
  sail_servo.attach(SAIL_SERVO_PIN);
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

void loop() {
  angle wind = windsensor.relative();
  uangle bearing = compass.bearing();
  move_rudder();
  sail.set_position(wind);
  gps_time_to_read = millis();
  gpsResult gpsData = gps.data(gps_wait);
  gps_time_to_read = millis() - gps_time_to_read;
  if (gpsData.fix == -1) {
    gps_wait = gps_wait + 5000;
  }
  else if (gps_wait > 2000) {
    gps_wait = gps_wait - 1000;
  }
  String msg = "All items, GPS took (ms): ";
  msg = msg + gps_time_to_read;
  logger.info(&gpsData, wind, bearing, msg);

  delay(5000);
}
