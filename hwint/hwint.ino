#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
// pick between loggers here
#include <DisplayLogger.h>
//#include <SerialLogger.h>

using namespace Angle;
using namespace Position;

WindSensor windsensor;
Compass compass;
Logger logger;
position current_position;

void setup() {
  logger.begin();
  windsensor.begin();
  compass.begin();
}

void loop() {
  angle wind = windsensor.relative();
  uangle bearing = compass.bearing();
  logger.info(&current_position, wind, bearing, "Testing wind and bearing and a bit");

  delay(1000);
}
