#ifndef MServo_h
#define MServo_h

#include "Arduino.h"
#include "Boat.h"

#define SERVO_MIN 2000
#define SERVO_MAX 10000

class MServo
{
  public:
    MServo();
    MServo(Boat *boatp);
    void begin();
    void write(uint8_t channel, uint16_t angle);  // unsigned angle
    void setSpeed(uint8_t channel, uint16_t speed);
    void setAccel(uint8_t channel, uint16_t accel);

  private:
    Boat *boat;
};

#endif
