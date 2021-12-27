#include <SPI.h>
#include <SD.h>
#include <MServo.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Gps.h>
#include <SDLogger.h>

#define CHIP_SELECT 4
#define RUDDER_CHANNEL 0
#define SAIL_CHANNEL 1

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "new data";
MServo servo;
Compass compass;
WindSensor wind;
Timer timer;
Gps gps(&timer);
gpsResult gpsReading;
SDLogger logger(&gps, &wind, &compass);

char buf[20];

void setup() {

  gps.begin();
  wind.begin();
  delay(50);
  compass.begin();

  servo.begin();
  logger.begin()
}

void loop() {

  logger.banner("ServoSD start");

  while (true) {

    logger.msg("ServoSD start cycle");
    servo.setSpeed(RUDDER_CHANNEL, 20);
    servo.setAccel(RUDDER_CHANNEL, 0);

    servo.write(SAIL_CHANNEL, 45);

    logger.msg("ServoSD Rudder");

    servo.write(RUDDER_CHANNEL, 45);
    delay(1000);
    servo.write(RUDDER_CHANNEL, 135);
    delay(1000);


    servo.write(SAIL_CHANNEL, 135);
    logger.msg("ServoSD End cycle");

    gps.data(500, &gpsReading);
    short bearing = compass.bearing();
    short relative_wind = wind.relative();

    delay(2000);
  }

  // gps.data(500, &gpsReading);
  // short bearing = compass.bearing();
  // short relative_wind = wind.relative();
  // bearing = compass.bearing();
  // relative_wind = wind.relative();
  // bearing = compass.bearing();
  // relative_wind = wind.relative();
  //
  // dataFile = SD.open("datalog.txt", FILE_WRITE);
  //
  // if (dataFile) {
  //   dataFile.print(gpsReading.unixTime); dataFile.print(",");
  //   dataFile.print(gpsReading.pos.latitude,5); dataFile.print(",");
  //   dataFile.print(gpsReading.pos.longitude,5); dataFile.print(",");
  //   dataFile.print(dataString);dataFile.print(",");
  //   dataFile.print(bearing);dataFile.print(",");
  //   dataFile.println(relative_wind);
  //   dataFile.close();
  // }

}
