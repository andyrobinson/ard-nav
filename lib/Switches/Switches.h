#ifndef Switches_h
#define Switches_h

#include "Arduino.h"

#define SWITCH1 7
#define SWITCH2 8
#define SWITCH4 9

class Switches
{
  public:
    Switches();
    void begin();
    uint8_t value();
};

#endif
