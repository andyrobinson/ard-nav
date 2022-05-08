#ifndef Battery_h
#define Battery_h

#include "Arduino.h"

#define LIPO1 A0
#define MAX_ANALOG 1024.0  // default 10 bits

class Battery
{
  public:
    Battery();
    float lipo1v();
};

#endif
