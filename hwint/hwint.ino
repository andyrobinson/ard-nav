#include <Arduino.h>
#include <Compass.h>
#include <WindSensor.h>
#include <Position.h>
#include <Rudder.h>
#include <Sail.h>
#include <Gps.h>



// include the GPS in the test?
#define SKIP_GPS
//#define SKIP_SERVO
//#define SKIP_DISPLAY

#ifndef SKIP_DISPLAY
// pick between loggers here
#include <DisplayLogger.h>
//#include <SerialLogger.h>
#endif

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

using namespace Angle;
using namespace Position;

#ifndef SKIP_SERVO
Servo rudder_servo;
Rudder rudder(&rudder_servo);
Servo sail_servo;
Rudder sail(&sail_servo);
#endif

Gps gps;
WindSensor windsensor;
Compass compass;
Logger logger;
position current_position;
gpsResult gpsReading;

void setup() {
  #ifndef SKIP_GPS
  gps.begin();
  #endif

  #ifndef SKIP_DISPLAY
  logger.begin();
  #endif

  windsensor.begin();
  compass.begin();

  #ifndef SKIP_SERVO
  rudder_servo.attach(RUDDER_SERVO_PIN);
  sail_servo.attach(SAIL_SERVO_PIN);
  #endif
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

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

void loop() {
  angle wind = windsensor.relative();
  uangle bearing = compass.bearing();

  #ifndef SKIP_SERVO
  move_rudder();
  sail.set_position(wind);
  #endif


  #ifndef SKIP_GPS
  long gps_time_to_read = 0;
  gps_time_to_read = millis();
  read_gps();
  gps_time_to_read = millis() - gps_time_to_read;
  #endif

  int freemem = freeMemory();

  char intbuf[16];
  itoa(freemem, intbuf, 10);
  char msg[40] = "mem ";
  strcat(msg, intbuf);

  itoa(gps_time_to_read, intbuf, 10);
  strcat(msg, " gps ");
  strcat(msg, intbuf);

  #ifndef SKIP_DISPLAY
  logger.info(&gpsReading, wind, bearing, msg);
  #endif

  delay(200);
}
