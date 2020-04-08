#ifndef SelfTest_h
#define SelfTest_h

#include "Position.h"
#include "Gps.h"
#include "WindSensor.h"
#include "Compass.h"
#include "Sail.h"
#include "Rudder.h"
#include "Timer.h"

#define NEXT_TEST   22
#define IN_TEST     45
#define FAILED      -45
#define PASSED      0
#define SERVO_WAIT  30

class SelfTest
{
  public:
    SelfTest();
    SelfTest(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Sail *sailp, Rudder *rudderp, Timer *timerp);
    void test();

  private:
    void next();
    void read_gps(gpsResult *result);
    Gps *gps;
    WindSensor *windsensor;
    Compass *compass;
    Sail *sail;
    Rudder *rudder;
    Timer *timer;
};

#endif
