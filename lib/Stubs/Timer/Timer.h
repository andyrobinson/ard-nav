#ifndef TimerStub_h
#define TimerStub_h

class Timer
{
  public:
    Timer();
    void wait(unsigned long milliseconds);
    long millis();
    void set_millis(long millis);

  private:
    long timer_millis;
};

#endif
