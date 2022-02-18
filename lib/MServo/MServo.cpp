#include "MServo.h"
#include "wiring_private.h"

// sercom2SerialPort on SERCOM 2, TX = pin 2, RX = pin 3
Uart Serial3(&sercom2, PIN_SERIAL2_RX, PIN_SERIAL2_TX, SERCOM_RX_PAD_1, UART_TX_PAD_2);

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

MicroMaestro maestro(Serial3);

MServo::MServo() {};

void MServo::begin() {
  Serial3.begin(9600);
  pinPeripheral(2, PIO_SERCOM);
  pinPeripheral(3, PIO_SERCOM_ALT);
}

void MServo::write(uint8_t channel, uint16_t angle) {
  angle = constrain(angle, 0, 180);

  uint16_t pulse_width = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  maestro.setTarget(channel, pulse_width);
}

void MServo::setSpeed(uint8_t channel, uint16_t speed) {
  maestro.setSpeed(channel, speed);
}

void MServo::setAccel(uint8_t channel, uint16_t accel) {
  maestro.setAcceleration(channel, accel);
}
