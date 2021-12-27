#include <MServo.h>
#include <Position.h>
#include <Compass.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <SDLogger.h>
#include <Utility.h>
#include <Routes.h>
#include <RotaryPID.h>
#include <Switches.h>
#include <version.h>

#define MAJOR_VERSION 99 // for test

WindSensor windsensor;
MServo servo;
Compass compass;
Timer timer;
Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass);
Sail sail(&servo);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {

  //   gps.begin();
  //   wind.begin();
  //   delay(50);
  //   compass.begin();
  //
  //   servo.begin();
  //   logger.begin();

  servo.begin();
  delay(1000);
  rudder.begin();
  sail.begin();
  compass.begin();
//  windsensor.begin();
//  delay(100); // to allow time for I2C
  gps.begin();
  logger.begin();
  switches.begin();
}


// #define CHIP_SELECT 4
// #define RUDDER_CHANNEL 0
// #define SAIL_CHANNEL 1

// int pos = 0;    // variable to store the servo position
// File dataFile;
// char dataString[20] = "new data";
// MServo servo;
// Compass compass;
// WindSensor wind;
// Timer timer;
// Gps gps(&timer);
// gpsResult gpsReading;
// SDLogger logger(&gps, &wind, &compass);
//
// char buf[20];
//
// void setup() {
//
// }

void loop() {

  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

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
    short relative_wind = windsensor.relative();

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
