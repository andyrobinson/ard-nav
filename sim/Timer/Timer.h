#ifndef TimerStub_h
#define TimerStub_h

class Timer
{
  public:
    Timer();
    void wait(unsigned long milliseconds);
    unsigned long elapsed();

  private:
    unsigned long theTime;

};

#endif
