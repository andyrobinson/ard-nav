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
  Serial.begin(9600);
  Serial.println(F("Initialising"));

  windsensor.begin();
  sail_servo.attach(SAIL_SERVO_PIN);
}

void log(int w, int s) {
  Serial.print("wind: ");
  Serial.print(w);
  Serial.print(" sail angle:");
  Serial.println(s);
}

void loop() {
  int printcount = 0;
  angle wind_angle = 0;
  angle servo_angle = 0;
  
  printcount = (printcount + 1) % 100;

  if (printcount == 0) {
    log(wind_angle, servo_angle);
  }

  wind_angle = windsensor.relative();
  sail.set_position(wind_angle);
  servo_angle = sail_servo.read();

  delay(10);
}
