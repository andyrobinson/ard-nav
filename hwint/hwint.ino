#include <Arduino.h>
// #include <Compass.h>
// #include <WindSensor.h>
// #include <Position.h>
// #include <Rudder.h>
// #include <Sail.h>
#include <Gps.h>

// include the GPS in the test?
// pick between loggers here
#include <DisplayLogger.h>
//#include <SerialLogger.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

// using namespace Angle;
// using namespace Position;

// Servo rudder_servo;
// Rudder rudder(&rudder_servo);
// Servo sail_servo;
// Rudder sail(&sail_servo);

Gps gps;
// WindSensor windsensor;
// Compass compass;
Logger logger;
gpsResult gpsReading;

void setup() {
  gps.begin();

  logger.begin();
  // windsensor.begin();
  // compass.begin();
  // rudder_servo.attach(RUDDER_SERVO_PIN);
  // sail_servo.attach(SAIL_SERVO_PIN);
}

// void move_rudder() {
//   static angle rudder_position = 0;
//   static short rudder_increment = 5;
//
//   rudder_position = rudder_position + rudder_increment;
//   if (abs(rudder_position) >= RUDDER_MAX_DISPLACEMENT) {
//     rudder_increment = -rudder_increment;
//   }
//   rudder.set_position(rudder_position);
// }

void read_gps() {
  static int gps_wait = 2000;
  gps.data(gps_wait, &gpsReading);
  if (gpsReading.fix == -1) {
    gps_wait = gps_wait + 5000;
  }
  else if (gps_wait > 2000) {
    gps_wait = gps_wait - 1000;
  }
}

extern "C" char* sbrk(int incr);
int freeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}

void loop() {
  angle wind = 22; //windsensor.relative();
  uangle bearing = 99; //compass.bearing();

  // move_rudder();
  // sail.set_position(wind);

  read_gps();
  int freemem = freeMemory();

  char intbuf[16];
  itoa(freemem, intbuf, 10);
  char msg[40] = "mem ";
  strcat(msg, intbuf);

  logger.info(&gpsReading, wind, bearing, msg);
  delay(500);
}
