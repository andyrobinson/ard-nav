#include <Gps.h>
#include <Timer.h>

Timer timer;
Gps gps(&timer);
gpsResult gpsReading;

void setup()
{
  while (!Serial);
  Serial.begin(19200);
  Serial.println("GPS library test!");

  gps.begin();
}

void loop()
{
  gps.data(50, &gpsReading);

  Serial.print(gpsReading.unixTime); Serial.print(",");
  Serial.print(gpsReading.pos.latitude,5); Serial.print(",");
  Serial.print(gpsReading.pos.longitude,5); Serial.print(",");
  Serial.print("err ");Serial.print(gpsReading.pos.error); Serial.print(",");
  Serial.print("fix ");Serial.print(gpsReading.fix); Serial.print(",");
  Serial.print("m/s ");Serial.print(gpsReading.mps); Serial.println("");
  Serial.print("cog ");Serial.print(gpsReading.cog); Serial.println("");

  timer.wait(1000);
}
