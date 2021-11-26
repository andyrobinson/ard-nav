#include <SPI.h>
#include <SD.h>
#include <PololuMaestro.h>
#include "wiring_private.h"

#define SERVO_TX 2
#define SERVO_RX 3
#define CHIP_SELECT 4

#define PIN_SERIAL2_RX       (3ul)
#define PIN_SERIAL2_TX       (2ul)

// sercom2SerialPort on SERCOM 2, TX = pin 2, RX = pin 3
Uart sercom2SerialPort (&sercom2, PIN_SERIAL2_RX, PIN_SERIAL2_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  sercom2SerialPort.IrqHandler();
}

MicroMaestro maestro(sercom2SerialPort);

int pos = 0;    // variable to store the servo position
File dataFile;
char dataString[20] = "new data";

void setup() {
  sercom2SerialPort.begin(9600);
  Serial.begin(9600);

  pinPeripheral(2, PIO_SERCOM);
  pinPeripheral(3, PIO_SERCOM_ALT);

  delay(1000);

  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT)) {
    // don't do anything more:
    while (1);
  }

}

void loop() {

  if (!sercom2SerialPort.available()) {
    Serial.println("serial port did not become available");
  }
  maestro.setSpeed(0, 30);
  maestro.setAcceleration(0,20);

  maestro.setTarget(0, 5000);
  delay(2000);

  maestro.setTarget(0, 7000);

  for (int i=0; i < 10; i++) {
    dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }

  delay(2000);

}
