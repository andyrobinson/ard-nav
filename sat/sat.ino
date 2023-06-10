#include <Position.h>
#include <I2C.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <Utility.h>
#include <RotaryPID.h>
#include <Switches.h>
//#include <SDLogger.h>
#include <SerialLogger.h>
#include <Battery.h>
#include <version.h>
#include <MServo.h>
#include <IridiumSBD.h>
#include <SatComm.h>

#define MAJOR_VERSION 3 // first successful reliable navigation
#define STARTUP_WAIT_FOR_FIX_MS 60000

char logmsg[40];

#define IRIDIUM_SERIAL Serial1
#define IRIDIUM_SLEEP_PIN 6

// Dependency injection

Timer timer;
Globe globe;
Switches switches;
I2C i2c;
MServo servo_control;

Battery battery(&analogRead, &timer);
WindSensor windsensor(&i2c);
Compass compass(&i2c, &timer);
Gps gps(&timer);

// Declare the IridiumSBD object (note SLEEP pin)
IridiumSBD modem(IRIDIUM_SERIAL, IRIDIUM_SLEEP_PIN);

//SDLogger logger(&gps, &windsensor, &compass, &battery, &switches, &timer, 0);
SerialLogger logger(&gps, &windsensor, &compass, &battery, &timer);

SatComm satcomm(&modem, &timer, &gps, &battery, &compass, &logger);
Sail sail(&servo_control);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches);
Rudder rudder(&servo_control);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &satcomm, &logger);

// Iridium callback, used when no-block waiting
bool ISBDCallback() {
    return helm.steer_and_continue();
}

void setup() {
  satcomm.begin();
  i2c.begin();
  servo_control.begin();
  rudder.begin();
  sail.begin();
  compass.begin();
  gps.begin();
  switches.begin();
  logger.begin();
  timer.wait(5000); // don't do anything, give it all a chance to settle
}

void loop() {
  // try and get a GPS fix before logging so that it goes in the same file
  gpsResult gps_data_ignored;
  gps.data(STARTUP_WAIT_FOR_FIX_MS, &gps_data_ignored);

  uangle direction = 0;

  while(true){
    logger.banner("Helm steering loop");
    helm.steer(direction, 30000L,{uadd(direction, (angle) 5),uadd(direction, (angle) -5)});
    direction = uadd(direction,5);
  };
}
