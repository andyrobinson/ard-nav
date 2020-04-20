#include "SelfTest.h"

SelfTest::SelfTest() {}

SelfTest::SelfTest(Gps *gpsp, WindSensor *windsensorp, Compass *compassp, Sail *sailp, Rudder *rudderp, Timer *timerp, Logger *loggerp):
  gps(gpsp), windsensor(windsensorp), compass(compassp), sail(sailp), rudder(rudderp), timer(timerp), logger(loggerp) {}

void SelfTest::next() {
  rudder->set_position(NEXT_TEST);
  timer->wait(SERVO_WAIT);
  rudder->set_position(IN_TEST);
  timer->wait(SERVO_WAIT);
}

void SelfTest::read_gps(gpsResult *result) {
  int gps_wait = 2000;
  boolean fix = false;
  while (gps_wait < 60000 && !fix) {
    gps->data(gps_wait, result);
    fix = result->fix < FIX_GPS;
    if (!fix) {
      gps_wait = gps_wait + 5000;
    }
  }
}

// we don't return a result because we just carry on.  This is only for a human
void SelfTest::test() {
  char pass[5]="pass";
  char fail[5]="fail";
  char logmsg[40];

  boolean ok = true;
  boolean allok = true;
  gpsResult gpsData;

  logger->msg("Test: starting");

  // rudder test
  rudder->set_position(-45);
  timer->wait(SERVO_WAIT);
  rudder->set_position(45);
  timer->wait(SERVO_WAIT);
  logger->msg("Test: Rudder complete");

  // WindSensor
  next();
  angle result = windsensor->relative();
  ok = (result != NO_WIND_VALUE);
  allok = (allok && ok);
  sprintf(logmsg, "Test: Wind %s", ok?pass:fail);
  logger->msg(logmsg);

  // Compass
  next();
  uangle bearing = compass->bearing();
  ok = (bearing >=0 && bearing <=360);
  allok = (allok && ok);
  sprintf(logmsg, "Test: Compass %s", ok?pass:fail);
  logger->msg(logmsg);

  // Sail
  next();
  sail->set_position(-120);
  timer->wait(1500);
  sail->set_position(120);
  timer->wait(1500);
  sail->set_position(0);
  logger->msg("Test: Sail complete");
  // cannot update ok

  // Gps
  next();
  read_gps(&gpsData);
  ok = (gpsData.fix >= FIX_GPS);
  allok = (allok && ok);
  sprintf(logmsg, "Test: Gps %s", ok?pass:fail);
  logger->msg(logmsg);

  next();
  logger->msg("Test: Complete");
  // rudder right, tests failed
  if (allok) {
    rudder->set_position(PASSED);
  }
  else {
    rudder->set_position(FAILED);
  }
  timer->wait(SERVO_WAIT);

}
