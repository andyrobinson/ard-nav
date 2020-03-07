#ifndef ServoStub_h
#define ServoStub_h

// slimmed down servo a la Arduino library
// https://github.com/arduino-libraries/Servo/blob/master/src/Servo.h
class Servo
{
  public:
    Servo();
    void write(int value);

    int write_last_called();

  private:
    int last_called;
};

#endif
