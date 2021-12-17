#include <SPI.h>
#include <SD.h>
#include <MServo.h>

#define CHIP_SELECT 4
#define RUDDER_CHANNEL 0
#define SAIL_CHANNEL 1

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = " new data";
MServo servo;

void setup() {

  servo.begin();

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    // don't do anything more:
    while (1);
  }

}

void slow_log() {
  for (int i=0; i<4; i++) {
    delay(300);
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(millis());
      dataFile.println(dataString);
      dataFile.close();
    }
  }
}

void loop() {

  servo.setSpeed(RUDDER_CHANNEL, 20);
  servo.setAccel(RUDDER_CHANNEL, 0);

  servo.write(SAIL_CHANNEL, 45);
  servo.write(RUDDER_CHANNEL, 45);
  slow_log();

  servo.write(RUDDER_CHANNEL, 135);
  slow_log();

  servo.write(SAIL_CHANNEL, 135);
  slow_log();
  slow_log();

}
