#include <Compass.h>
#include <CompassWire.h>
#include <WindSensor.h>
#include <WindSensorWire.h>
#include <Timer.h>
#include <Gps.h>
#include <SDLogger.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <RotaryPID.h>
#include <Switches.h>
#include <MServo.h>
#include <version.h>

#define MAJOR_VERSION 111

#define CHIP_SELECT 4

char dataString[20] = ", new data";
char logmsg[30];
MServo servo_control;

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Switches switches;
Timer timer;
CompassWire compass;
WindSensorWire wind;
Gps gps(&timer);
SDLogger logger(&gps, &wind, &compass);
Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo_control);
Helm helm(&rudder, &compass, &timer, &wind, &sail, &rotaryPID, &logger);

char buf[20];

void setup() {
  sercom3.setTimeoutInMicrosWIRE(25000ul, true);  // for new timeout
  servo_control.begin();

  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  wind.begin();
  delay(50);
  compass.begin();
  gps.begin();
  logger.begin();
  sail.begin();
  rudder.begin();
  switches.begin();
  Serial.println("Starting test");
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

}

void loop() {

    windrange range = {0, 359};
    helm.steer(90, 30000l, range);
    Serial.print("Compass: "); Serial.print(compass.bearing());
    Serial.print(" e: ");Serial.print(compass.err_percent());Serial.print(" | ");

    Serial.print("Wind: "); Serial.print(wind.relative());
    Serial.print(" e: ");Serial.println(wind.err_percent());

    logger.banner(dataString);

}
