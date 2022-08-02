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

    while (!Serial); // wait for Serial to be ready
    Serial.begin(19200);
    Serial.println("Starting");

    if (!SD.begin(CHIP_SELECT)) {
      // need to do something else?
      Serial.println("Card failed, or not present");
        while(true) {};
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

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }

}
