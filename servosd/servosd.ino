#include <SPI.h>
#include <SD.h>
#include <MServo.h>
#include <Compass.h>
#include <WindSensor.h>

#define CHIP_SELECT 4
#define RUDDER_CHANNEL 0
#define SAIL_CHANNEL 1

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "new data";
MServo servo;
Compass compass;
WindSensor wind;
char buf[20];

void setup() {

  wind.begin();
  delay(50);
  compass.begin();

  servo.begin();

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    // don't do anything more:
    while (1);
  }

}

void loop() {

  servo.setSpeed(RUDDER_CHANNEL, 20);
  servo.setAccel(RUDDER_CHANNEL, 0);

  servo.write(SAIL_CHANNEL, 45);

  servo.write(RUDDER_CHANNEL, 45);
  delay(1000);
  servo.write(RUDDER_CHANNEL, 135);
  delay(1000);

  servo.write(SAIL_CHANNEL, 135);
  delay(2000);

  dataFile = SD.open("datalog.txt", FILE_WRITE);

  short bearing compass.bearing();
  short wind = wind.relative();

  if (dataFile) {
    dataFile.print(dataString);dataFile.print(",");
    dataFile.print(bearing);dataFile.print(",");
    dataFile.println(wind);
    dataFile.close();
  }

}
