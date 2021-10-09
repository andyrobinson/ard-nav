#ifndef ServoStub_h
#define ServoStub_h

// slimmed down servo a la Arduino library
// https://github.com/arduino-libraries/Servo/blob/master/src/Servo.h
class VSServoSamd
{
  public:
    VSServoSamd();
    void write(int value);
    void write(int value, uint8_t speed);

    int write_last_called();

  private:
    int last_called;
};

#endif
