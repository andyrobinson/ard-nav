// Adafruit SSD1306 - Version: Latest
#include <Wire.h>
#include <WindSensor.h>
#include <math.h>
#include <Servo.h>
#include <Angle.h>
#include <Sail.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

WindSensor windsensor;
Servo sail_servo;
Sail sail(&sail_servo);

void setup() {
  windsensor.begin();
  sail_servo.attach(SAIL_SERVO_PIN);
}

void loop() {
  angle wind_angle = 0;
  angle servo_angle = 0;

  wind_angle = windsensor.relative();
  sail.set_position(wind_angle);

  delay(10);
}
