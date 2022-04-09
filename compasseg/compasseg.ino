#include <version.h>
#include <SPI.h>
#include <Wire.h>
#include <Compass.h>
#include <MServo.h>
#include <WindSensor.h>
#include <Timer.h>
#include <Gps.h>
#include <SDLogger.h>
#include <Position.h>
#include <Globe.h>
#include <Sail.h>
#include <Rudder.h>
#include <Helm.h>
#include <Tacker.h>
#include <Navigator.h>
#include <Captain.h>
#include <Utility.h>
#include <Routes.h>
#include <Switches.h>
#include <RotaryPID.h>
#include "wiring_private.h"


#define MAJOR_VERSION         111
#define RUDDER_CHANNEL      0
#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

using namespace Angle;

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

WindSensor windsensor;
Compass compass;
Timer timer;
Globe globe;
Switches switches;
char logmsg[50];

// Dependency injection
MicroMaestro maestrolib(Serial3);
MServo servo_controller(&maestrolib);
Gps gps(&timer);
SDLogger logger(&gps, &windsensor, &compass);
Sail sail(&servo_controller);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Rudder rudder(&servo_controller);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

MagResult rbearing;
uint16_t rudder_pos = 135;
uint16_t rudder_diff = 90;

void setup() {
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);
  servo_controller.setSpeed(RUDDER_CHANNEL, 15);
  servo_controller.setAccel(RUDDER_CHANNEL, 0);

  Wire.begin();  // no longer included in compass or windsensor
  compass.begin();
  gps.begin();
  logger.begin();

  // sail_servo.attach(SAIL_SERVO_PIN);
  // rudder_servo.attach(RUDDER_SERVO_PIN);

  switches.begin();
}

void loop() {
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION); logger.banner(logmsg);
  sprintf(logmsg, "Switches %3d", switches.value()); logger.banner(logmsg);

  uint8_t sw = switches.value() & 3; // four routes configurable
  route journey = plattfields[sw];

  // TEMP TEST
  while(true){
    // lots of exercise for I2C
    for (int i=0; i< 110;i++) {
      rbearing = compass.raw_bearing();
      compass.bearing(); // this adds a significant amount of time
      delay(5);
    }

    servo_controller.write(RUDDER_CHANNEL, rudder_pos);

    logger.banner("compass eg");

    rudder_diff = -rudder_diff;
    rudder_pos = rudder_pos + rudder_diff;
  }
  // END TEMP TEST

  //  captain.voyage(journey.waypoints, journey.length);

  logger.banner("Finished Navigation :-)");

  while(true){};
}
