#include <Compass.h>
#include <Position.h>
#include <SerialLogger.h>

// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
//#define PIN_SERIAL2_RX       (34ul)               // Pin description number for PIO_SERCOM on D12
//#define PIN_SERIAL2_TX       (36ul)               // Pin description number for PIO_SERCOM on D10
//#define PAD_SERIAL2_TX       (UART_TX_PAD_2)      // SERCOM pad 2
//#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3

// Instantiate the Serial2 class
// Uart Serial2(&sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

// void SERCOM1_Handler()    // Interrupt handler for SERCOM1
// {
//   Serial2.IrqHandler();
// }

using namespace Angle;
using namespace Position;
// WindSensor windsensor;
Compass compass;
Logger logger;
position current_position;

void setup() {
  while (!Serial); // wait for Serial to be ready

  Serial.begin(19200);
  logger.begin();
  // windsensor.begin();
  compass.begin();

  // if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }

  // Serial2.begin(9600);
  // delay(2000);

  // Serial.println("Get version!");
  // Serial2.println(PMTK_Q_RELEASE);

  // you can send various commands to get it started
  //mySerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Serial2.println(PMTK_SET_NMEA_OUTPUT_ALLDATA);

  // Serial2.println(PMTK_SET_NMEA_UPDATE_1HZ);

}

void loop() {

  // if (Serial2.available()) {
  //   //byte byteRead = Serial2.read();
  //   char c = Serial2.read();
  //   Serial.write(c);
  // }

  // angle wind = windsensor.relative();
  uangle heading = compass.bearing();
  logger.info(&current_position, 0, heading, "Testing ...");

  delay(1000);

}
