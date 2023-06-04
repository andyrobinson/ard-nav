#ifndef Timer_h
#define Timer_h
#include "time.h"

class Timer
{
  public:
    Timer();
    void wait(unsigned long millisecs);
    unsigned long milliseconds();

    // time functions
    time_t now(); // unix time
    tm *nowTm(); // structured time
    void setTime(time_t t);
    bool isTimeSet();
    time_t lastRestart;

  private:
    unsigned long lastSetMillis;
    time_t lastSetTime;
    bool isSet;

};

#endif
