#ifndef MServo_h
#define MServo_h

#include "Arduino.h"

#define SERVO_TX 2
#define SERVO_RX 3

#define PIN_SERIAL2_RX       (3ul)
#define PIN_SERIAL2_TX       (2ul)
#define SERVO_MIN 2000
#define SERVO_MAX 10000

// Servo via Pololu Maestro controller, with similar interface to Servo
class MServo
{
  public:
    MServo();
    void write(uint8_t channel, uint16_t angle);  // unsigned angle
    void setSpeed(uint8_t channel, uint16_t speed);
    void setAccel(uint8_t channel, uint16_t accel);
    void begin();
};

#endif
