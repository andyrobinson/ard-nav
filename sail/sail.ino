// Adafruit SSD1306 - Version: Latest
#include <Wire.h>
#include <WindSensor.h>
#include <math.h>
#include <Servo.h>
#include <Angle.h>
#include <Sail.h>

WindSensor windsensor;
Servo sail_servo;
Sail sail;

int printcount = 0;
angle wind_angle = 0;
angle servo_angle = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initialising"));

  sail = Sail();
  windsensor.begin();
  sail_servo.attach(4);
}

void log(int w, int s) {
  Serial.print("wind: ");
  Serial.print(w);
  Serial.print(" sail angle:");
  Serial.println(s);
}

angle safe_servo_angle (angle servo_angle) {
  angle result = servo_angle;
  if (servo_angle > 90) {
    result = 90;
  }
  else if (servo_angle < -90) {
    result = -90;
  }
  return 90 - result;
}

void loop() {
  printcount = (printcount + 1) % 100;

  if (printcount == 0) {
    log(wind_angle, servo_angle);
  }

  wind_angle = windsensor.relative();
  servo_angle = sail.sail_position(wind_angle);
  sail_servo.write(safe_servo_angle(servo_angle));

  delay(10);
}