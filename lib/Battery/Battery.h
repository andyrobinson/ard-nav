#ifndef Battery_h
#define Battery_h

#include "Arduino.h"

#define LIPO1 A0
#define MAX_ANALOG 1024.0  // default 10 bits
#define SAMPLES 10

class Battery
{
  public:
    Battery();
    float lipo1v();
    float lipo1avgv();

  private:
    float average1;
    int readings1;
    void add_reading();

};

#endif
