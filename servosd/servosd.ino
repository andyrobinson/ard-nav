#include <SPI.h>
#include <SD.h>
#include <MServo.h>

#define CHIP_SELECT 4
#define RUDDER_CHANNEL 0
#define SAIL_CHANNEL 1

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "new data";
MServo servo;

void setup() {

  servo.begin();

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    // don't do anything more:
    while (1);
  }

}

void loop() {

  servo.setSpeed(RUDDER_CHANNEL, 30);
  servo.setAccel(RUDDER_CHANNEL, 20);

  Serial.println("moving rudder");

  servo.write(RUDDER_CHANNEL, 45);
  delay(2000);
  servo.write(RUDDER_CHANNEL, 90);
  delay(2000);
  servo.write(RUDDER_CHANNEL, 135);
  delay(2000);
  servo.write(RUDDER_CHANNEL, 90);
  delay(2000);

  Serial.println("moving sail");

  servo.write(SAIL_CHANNEL, 0);
  delay(6000);
  servo.write(SAIL_CHANNEL, 45);
  delay(4000);
  servo.write(SAIL_CHANNEL, 90);
  delay(4000);
  servo.write(SAIL_CHANNEL, 135);
  delay(4000);
  servo.write(SAIL_CHANNEL, 180);
  delay(4000);
  servo.write(SAIL_CHANNEL, 90);

  delay(10000);

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }

  delay(2000);

}
