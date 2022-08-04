#include <version.h>
#include <Compass.h>
#include <I2C.h>
#include <Timer.h>
#include <Wire.h>

#define MAJOR_VERSION         111

char logmsg[50] = "stuff";
MagResult rawbearing;
MagResult rawaccel;
uangle bearing;
int errors;

Timer timer;
I2C i2c;
Compass compass(&i2c, &timer);

void setup() {
  Wire.begin();
  i2c.begin();
  compass.begin();

  while (!Serial);
  Serial.begin(19200);

  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  Serial.println(logmsg);
}

void loop() {

    rawbearing = compass.raw_bearing();
    rawaccel = compass.raw_accel();
    bearing = compass.bearing();
    errors = i2c.err_percent();

    Serial.print(millis()/1000); Serial.print("  [");
    Serial.print(bearing); Serial.print("],");
    Serial.print(errors); Serial.print(",{");
    Serial.print(rawbearing.x); Serial.print(",");
    Serial.print(rawbearing.y); Serial.print(",");
    Serial.print(rawbearing.z); Serial.print("},{");
    Serial.print(rawaccel.x); Serial.print(",");
    Serial.print(rawaccel.y); Serial.print(",");
    Serial.print(rawaccel.z); Serial.print("}");

    if (errors >= 100) {
      Serial.print(" ** I2C Failure **");
    }

    Serial.println("");

    timer.wait(1000);
}
