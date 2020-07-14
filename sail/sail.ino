// Adafruit SSD1306 - Version: Latest
#include <Wire.h>
#include <WindSensor.h>
#include <Compass.h>
#include <Gps.h>
#include <SDLogger.h>
#include <math.h>
#include <Servo.h>
#include <Angle.h>
#include <Sail.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

WindSensor windsensor;
Servo sail_servo;
Servo rudder_servo;
Sail sail(&sail_servo);
Compass compass;
Gps gps;
SDLogger logger(&gps, &windsensor, &compass);

void setup() {
  windsensor.begin();
  compass.begin();
  gps.begin();
  logger.begin();

  sail_servo.attach(SAIL_SERVO_PIN);
  rudder_servo.attach(RUDDER_SERVO_PIN); //stops jitter
}

char logmsg[22];

void loop() {
  angle wind_angle = 0;
  angle servo_angle = 0;

  wind_angle = windsensor.relative();
  sail.set_position(wind_angle);

  sprintf(logmsg, "Sail %4dW,%4dS", wind_angle, sail_servo.read()); logger.msg(logmsg);

  delay(100);
}
