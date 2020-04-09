#include <Arduino.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
#include <Rudder.h>
#include <Sail.h>
#include <Gps.h>

// pick between loggers here
//#include <DisplayLogger.h>
#include <SerialLogger.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5
//#define SKIP_GPS

using namespace Angle;
using namespace Position;

Servo rudder_servo;
Rudder rudder(&rudder_servo);
Servo sail_servo;
Rudder sail(&sail_servo);

Gps gps;
WindSensor windsensor;
Compass compass;
Logger logger;
position current_position;
gpsResult gpsReading;

void setup() {
  gps.begin();
  logger.begin();
  windsensor.begin();
  compass.begin();
  rudder_servo.attach(RUDDER_SERVO_PIN);
  sail_servo.attach(SAIL_SERVO_PIN);
}

void move_rudder() {
  static angle rudder_position = 0;
  static short rudder_increment = 5;

  rudder_position = rudder_position + rudder_increment;
  if (abs(rudder_position) >= RUDDER_MAX_DISPLACEMENT) {
    rudder_increment = -rudder_increment;
  }
  rudder.set_position(rudder_position);
}

gpsResult read_gps() {
  static int gps_wait = 2000;
  gpsResult gpsData = gps.data(gps_wait);
  if (gpsData.fix == -1) {
    gps_wait = gps_wait + 5000;
  }
  else if (gps_wait > 2000) {
    gps_wait = gps_wait - 1000;
  }
  return gpsData;
}

void loop() {
  angle wind = windsensor.relative();
  uangle bearing = compass.bearing();
  move_rudder();
  sail.set_position(wind);
  long gps_time_to_read = 0;

  #ifndef SKIP_GPS
  gps_time_to_read = millis();
  gpsReading = read_gps();
  gps_time_to_read = millis() - gps_time_to_read;
  #endif

  char gpstime[16];
  itoa(gps_time_to_read, gpstime, 10);
  char msg[40] = "Log, gps (ms): ";
  strcat(msg, gpstime);
  logger.info(&gpsReading, wind, bearing, msg);

  delay(500);
}
