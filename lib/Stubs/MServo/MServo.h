#ifndef MServoStub_h
#define MServoStub_h

class MServo
{
  public:
    MServo();
    void write(uint8_t channel, uint16_t angle);  // unsigned angle
    void setSpeed(uint8_t channel, uint16_t speed);
    void setAccel(uint8_t channel, uint16_t accel);

    uint8_t channel_last_called;
    uint16_t angle_last_called;
    uint16_t accel_last_called;
    uint16_t speed_last_called;
};

#endif
