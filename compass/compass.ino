#include <Compass.h>
#include <CompassWire.h>
#include <WindSensor.h>
#include <WindSensorWire.h>
#include <Timer.h>
#include <Gps.h>
#include <SDLogger.h>
#include <MServo.h>

#define CHIP_SELECT 4

char dataString[20] = ", new data";
MServo servo;

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Timer timer;
CompassWire compass;
WindSensorWire wind;
Gps gps(&timer);
SDLogger logger(&gps, &wind, &compass);

char buf[20];

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  wind.begin();
  delay(50);
  compass.begin();
  gps.begin();
  logger.begin();
  Serial.println("Starting test");
}

void loop() {
    for (int i=0; i< 100;i++) {
      compass.bearing();
      wind.relative();
      delay(5);
    }

    Serial.print("Compass: "); Serial.print(compass.bearing());
    Serial.print(" e: ");Serial.print(compass.err_percent());Serial.print(" | ");

    Serial.print("Wind: "); Serial.print(wind.relative());
    Serial.print(" e: ");Serial.println(wind.err_percent());

    logger.banner(dataString);

    // m = compass.raw_accel();
    // Serial.print("{");
    // Serial.print(m.x); Serial.print(",");
    // Serial.print(m.y); Serial.print(",");
    // Serial.print(m.z);
    // Serial.print("} ");
    // m = compass.raw_bearing();
    // Serial.print("{");
    // Serial.print(m.x); Serial.print(",");
    // Serial.print(m.y); Serial.print(",");
    // Serial.print(m.z);
    // Serial.println("} ");
}
