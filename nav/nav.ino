#include <Servo.h>
#include <Position.h>
#include <Compass.h>
#include <Timer.h>
#include <Gps.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <SelfTest.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <DisplayLogger.h>
#include <SerialLogger.h>
#include <SDLogger.h>
#include <MultiLogger.h>

#define SAIL_SERVO_PIN 6
#define RUDDER_SERVO_PIN 5

// test
// waypoint route[] = {{"Chorlton", {53.44096, -2.27235, 100.0}},
//                     {"Up North", {80.0, -2.27, 0.1}},
//                     {"Dn South", {10.0, -2.27, 0.1}}};

// Chorlton Park
// waypoint route[] = {{"Pk Beg", {53.43485, -2.26893, 5}},
//                     {"A",   {53.43553, -2.27068, 5}},
//                     {"B",   {53.43519, -2.27118, 5}},
//                     {"C",   {53.43452, -2.26970, 5}},
//                     {"D",   {53.43518, -2.26972, 5}},
//                     {"E",   {53.43476, -2.27023, 5}},
//                     {"Pk End", {53.43485, -2.26893, 5}}};

// Hough end
waypoint route[] = {{"Hough", {53.43772, -2.24809, 5}},
                    {"B",     {53.437078, -2.24770188, 5}},
                    {"C",     {53.43746, -2.24735, 5}},
                    {"D",     {53.43742, -2.24842, 5}},
                    {"Hough", {53.43772, -2.24809, 5}}};

#define routeLength (sizeof(route)/sizeof(route[0])) //array size

WindSensor windsensor;
Servo sail_servo;
Servo rudder_servo;
Compass compass;
Timer timer;
Gps gps;
Globe globe;

// Dependency injection
SDLogger sdlogger(&gps, &windsensor, &compass);
DisplayLogger displaylogger(&gps, &windsensor, &compass);
SerialLogger seriallogger(&gps, &windsensor, &compass);
Logger* loggers[] = {&sdlogger};
MultiLogger logger(&gps, &windsensor, &compass, loggers, 1);

Sail sail(&sail_servo);
Rudder rudder(&rudder_servo);
SelfTest selftest(&gps, &windsensor, &compass, &sail, &rudder, &timer, &logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

void setup() {
  sail_servo.attach(SAIL_SERVO_PIN);
  rudder_servo.attach(RUDDER_SERVO_PIN);

  windsensor.begin();
  compass.begin();
  gps.begin();
  logger.begin();
}

void loop() {
  selftest.test();
  captain.voyage(route,routeLength);
  logger.banner("Done");
  while(true){};
}
