#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
#include <Rudder.h>
#include <Sail.h>
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

WindSensor windsensor;
Compass compass;
Logger logger;
position current_position;

void setup() {
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
  logger.info(&current_position, wind, bearing, "All items");

  delay(1000);
}
