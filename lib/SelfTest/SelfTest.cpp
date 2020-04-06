#include "SelfTest.h"

SelfTest::SelfTest() {}

SelfTest(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Sail *sailp, Rudder *rudderp);
  gps(gpsp), windsensor(windsensorp), compass(compassp), sail(sailp), rudder(rudderp) {}

void SelfTest::test() {
  // rudder full right, then
  // rudder left, in test

    // WindSensor
    // Compass
    // Sail
    // Gps - how do we set this going and carry on?
    
  // rudder right, tests failed


  // rudder centre, good to go
}
