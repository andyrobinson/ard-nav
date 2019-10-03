// Adafruit SSD1306 - Version: Latest
#include <Wire.h>
#include <WindSensor.h>
#include <math.h>
#include <Servo.h>

WindSensor windsensor;
Servo servo;  

int servoposition = 0; 
int printcount = 0;
int angle = 0;

void setup() {
  Serial.begin(9600);

  Serial.println(F("Initialising"));

  windsensor.begin();
  servo.attach(4);
}

void loop() {
  printcount = (printcount + 1) % 100;
  if (printcount == 0) {
    Serial.print("reading: ");
    Serial.println(angle);
  }
  angle = windsensor.angle();
  servoposition = angle/2;
  servo.write(servoposition); 
  delay(10);
}
