#include <SPI.h>
#include <SD.h>
#include <Servo.h>

#define SERVO_PIN 5
#define SERVO_IGNORE 6
#define CHIP_SELECT 4

Servo servo;
Servo ignore;
int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "some data";

void setup() {
  servo.attach(SERVO_PIN);
  ignore.attach(SERVO_IGNORE);

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    // don't do anything more:
    while (1);
  }

}

void loop() {
  for (pos = 45; pos <= 135; pos += 1) {
    servo.write(pos);
    delay(15);
  }

  for (pos = 135; pos >= 45; pos -= 1) {
    servo.write(pos);
    delay(15);
  }

  for (int i=0; i < 10; i++) {
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
    }
  }

}
