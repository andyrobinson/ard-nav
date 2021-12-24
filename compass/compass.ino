#include <Compass.h>
#include <WindSensor.h>

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Compass compass;
WindSensor wind;
char buf[20];

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  wind.begin();
  delay(50);
  compass.begin();
  Serial.println("Starting test");
}

void loop() {
    // MagResult m;
    // for (int i=0; i< 200;i++) {
    //   compass.bearing();
    //   delay(10);
    // }

    delay(1000);
    Serial.print("Compass: "); Serial.print(compass.bearing());Serial.print(" ");
    Serial.print("Wind: "); Serial.println(wind.relative());

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
