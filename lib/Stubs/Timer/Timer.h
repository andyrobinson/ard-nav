#ifndef TimerStub_h
#define TimerStub_h

#include "time.h"

class Timer
{
  public:
    Timer();
    void wait(unsigned long milliseconds);
    unsigned long milliseconds();
    void set_millis(long millis);

    // time functions
    time_t now(); // unix time
    tm *nowTm(); // structured time
    void setTime(time_t t);
    bool isTimeSet();
    time_t lastRestart;
    void reset();

  private:
    unsigned long timer_millis;
    unsigned long set_time;
    bool is_set;
};

#endif
