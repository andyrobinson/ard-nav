#include "MServo.h"
#include "wiring_private.h"

// Serial3 on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

MServo::MServo():maestro(MicroMaestro(Serial3)){};   // probably OK because constructor does not do anything with this

void MServo::begin() {
  // must come first
  pinPeripheral(PIN_SERIAL3_TX, PIO_SERCOM);
  pinPeripheral(PIN_SERIAL3_RX, PIO_SERCOM_ALT);
  Serial3.begin(9600);
}

void MServo::write(uint8_t channel, uint16_t angle) {
  angle = constrain(angle, 0, 180);

  uint16_t pulse_width = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  // pulse_width = constrain(pulse_width, 4010, 7990);
  Serial.print(" p");Serial.print(pulse_width);Serial.print(" ");
  maestro.setTarget(channel, pulse_width);
}

void MServo::setSpeed(uint8_t channel, uint16_t speed) {
  maestro.setSpeed(channel, speed);
}

void MServo::setAccel(uint8_t channel, uint16_t accel) {
  maestro.setAcceleration(channel, accel);
}
