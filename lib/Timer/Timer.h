#ifndef Timer_h
#define Timer_h

class Timer
{
  public:
    Timer();
    void wait(unsigned long milliseconds);
    long millis();
};

#endif
