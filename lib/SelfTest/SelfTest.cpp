#include "SelfTest.h"

SelfTest::SelfTest() {}

SelfTest::SelfTest(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Sail *sailp, Rudder *rudderp, Timer *timerp):
  gps(gpsp), windsensor(windsensorp), compass(compassp), sail(sailp), rudder(rudderp), timer(timerp) {}

void SelfTest::next() {
  rudder->set_position(NEXT_TEST);
  timer->wait(SERVO_WAIT);
  rudder->set_position(IN_TEST);
  timer->wait(SERVO_WAIT);
}

void SelfTest::read_gps(gpsResult *result) {
  int gps_wait = 2000;
  boolean fix = false;
  while (gps_wait < 30000 && !fix) {
    gps->data(gps_wait, result);
    fix = result->fix < FIX_GPS;
    if (!fix) {
      gps_wait = gps_wait + 5000;
    }
  }
}

// we don't return a result because we just carry on.  This is only for a human
void SelfTest::test() {
  boolean ok = true;
  gpsResult gpsData;

  // rudder test
  rudder->set_position(-45);
  timer->wait(SERVO_WAIT);
  rudder->set_position(45);
  timer->wait(SERVO_WAIT);

  // WindSensor
  next();
  angle result = windsensor->relative();
  ok = (ok && result != NO_WIND_VALUE);

  // Compass
  next();
  uangle bearing = compass->bearing();
  ok = (ok && (bearing >=0 && bearing <=360));

  // Sail
  next();
  sail->set_position(-120);
  timer->wait(1500);
  sail->set_position(120);
  timer->wait(1500);
  sail->set_position(0);
  // cannot update ok

  // Gps
  next();
  read_gps(&gpsData);
  ok = (ok && gpsData.fix >= FIX_GPS);

  // rudder right, tests failed
  if (ok) {
    rudder->set_position(PASSED);
  }
  else {
    rudder->set_position(FAILED);
  }
  timer->wait(SERVO_WAIT);

}
