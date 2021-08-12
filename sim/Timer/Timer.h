#ifndef TimerStub_h
#define TimerStub_h

#include "Boat.h"

class Timer
{
  public:
    Timer(Boat *boatp);
    void wait(unsigned long milliseconds);
    unsigned long elapsed();

  private:
    unsigned long theTime;
    Boat *boat;

};

#endif
