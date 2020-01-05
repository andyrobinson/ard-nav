#include <SPI.h>
#include <Wire.h>
#include <Compass.h>

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Compass compass;
char buf[20];

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  compass.begin();
}

void loop() {

 sprintf(buf, "Bearing: %d",compass.bearing());
 Serial.println(buf);

 delay(1000);

}
