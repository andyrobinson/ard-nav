#include <Compass.h>

// Simple test for the compass library - should
// manually try it at all compass points, and with tilt

Compass compass;
char buf[20];

void setup() {
  while (!Serial); // wait for Serial to be ready
  Serial.begin(19200);
  compass.begin();
  Serial.println("Starting test");
}

void loop() {
  short b;
  for (int i=0;i<100;i++) {
    b = compass.bearing();
    delay(100);
  }
  Serial.print(b);
  Serial.println(" tick");

}
