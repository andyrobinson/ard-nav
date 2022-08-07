#ifndef Timer_h
#define Timer_h

class Timer
{
  public:
    Timer();
    void wait(unsigned long millisecs);
    long milliseconds();
};

#endif
