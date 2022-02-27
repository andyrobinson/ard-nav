#include <Compass.h>
#include <CompassWire.h>
#include <Rudder.h>
#include <MServo.h>
#include <version.h>
#include <Angle.h>

#define MAJOR_VERSION 111

using namespace Angle;

char logmsg[50] = "stuff";
MServo servo_control;
CompassWire compass;
Rudder rudder(&servo_control);
angle bearing;
int tol;
int compasserr;

void setup() {
  while (!Serial);
  Serial.begin(19200);

  sercom3.setTimeoutInMicrosWIRE(25000ul, true);  // for new timeout
  servo_control.begin();

  compass.begin();
  rudder.begin();
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  Serial.println(logmsg);
}

void loop() {

    angle TEMP_RUDDER = 25;

    // lots of exercise for I2C
    for (int i=0; i< 200;i++) {
      bearing = compass.bearing();
      compasserr = compass.err_percent();
      tol = compass.timeout_location();
      delay(5);
    }

    if (compass.err_percent() >= 10000) {
      Serial.println("** I2C Failure **");
      while (true) {};
    }

    rudder.set_position(TEMP_RUDDER);

    Serial.print(millis()/1000); Serial.print(",");
    Serial.print(bearing); Serial.print(",");
    Serial.print(compasserr); Serial.print(",");
    Serial.print(tol); Serial.print(",");
    Serial.println("logging");

    TEMP_RUDDER = -TEMP_RUDDER;

}
