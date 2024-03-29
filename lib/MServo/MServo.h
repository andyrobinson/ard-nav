#ifndef MServo_h
#define MServo_h

#include "Arduino.h"
#include "PololuMaestro.h"

#define PIN_SERIAL3_RX       (3ul)
#define PIN_SERIAL3_TX       (2ul)

#define SERVO_MIN 2000
#define SERVO_MAX 10000

// Servo via Pololu Maestro controller, with similar interface to Servo
class MServo
{
  public:
    MServo();
    void begin();
    void write(uint8_t channel, uint16_t angle);  // unsigned angle
    void setSpeed(uint8_t channel, uint16_t speed);
    void setAccel(uint8_t channel, uint16_t accel);

  private:
    MicroMaestro maestro;
};

#endif
