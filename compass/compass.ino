#include <Compass.h>
#include <CompassWire.h>
#include <version.h>
#include <Angle.h>
#include <wiring_private.h>

#define MAJOR_VERSION         111
#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

using namespace Angle;

char logmsg[50] = "stuff";
CompassWire compass;
MagResult rbearing;
MagResult raccel;
int tol;
int compasserr;


// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

void setup() {
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);

  while (!Serial);
  Serial.begin(19200);

  sercom3.setTimeoutInMicrosWIRE(25000ul, true);  // for new timeout

  compass.begin();
  sprintf(logmsg, "Starting v%3d.%4d", MAJOR_VERSION, MINOR_VERSION);
  Serial.println(logmsg);
}

void loop() {

    angle TEMP_RUDDER = 25;

    // lots of exercise for I2C
    for (int i=0; i< 200;i++) {
      rbearing = compass.raw_bearing();
      raccel = compass.raw_accel();
      compasserr = compass.err_percent();
      tol = compass.timeout_location();
      delay(5);
    }

    Serial3.write(TEMP_RUDDER);

    Serial.print(millis()/1000); Serial.print(",(");
    Serial.print(rbearing.x); Serial.print(",");
    Serial.print(rbearing.y); Serial.print(",");
    Serial.print(rbearing.z); Serial.print("),(");
    Serial.print(raccel.x); Serial.print(",");
    Serial.print(raccel.y); Serial.print(",");
    Serial.print(raccel.z); Serial.print("),");
    Serial.print(compasserr); Serial.print(",");
    Serial.print(tol); Serial.print(",");
    Serial.println("logging");

    if (compass.err_percent() >= 10000) {
      Serial.println("** I2C Failure **");
      while (true) {};
    }

    TEMP_RUDDER = -TEMP_RUDDER;

}
