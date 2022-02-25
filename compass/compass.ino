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

char logmsg[50] = "stuff";
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
Rudder rudder(&servo_control);

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
  Serial.println("Starting test");
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);

}

void loop() {
    Serial.println("Loop");

    long remaining = 30000l;

    angle TEMP_RUDDER = 25;
    angle TEMP_RELATIVE_WIND = 100;
    int SAIL_COUNT = 0;

    if (compass.err_percent() >= 10000 && wind.err_percent() >= 10000) {
      Serial.println("I2C failure");
      sprintf(logmsg, "** I2C Failure **"); logger.banner(logmsg);
      while (true) {};
    }

    while (remaining > 0) {
      // lots of exercise for I2C
      for (int i=0; i< 100;i++) {
        compass.bearing();
        wind.relative();
        delay(5);
      }

      angle current_heading = compass.bearing();
      Serial.print("Steering ..."); Serial.println(current_heading);

      rudder.set_position(TEMP_RUDDER);

      sprintf(logmsg, "%4d", TEMP_RUDDER); logger.msg(logmsg);
      TEMP_RUDDER = -TEMP_RUDDER;

      remaining = remaining - 800;
    }

}
