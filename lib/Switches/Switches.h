#ifndef Switches_h
#define Switches_h

#include "Arduino.h"
#include "Utility.h"

#define SWITCH1 7
#define SWITCH2 8
#define SWITCH4 9
#define RCINPUT A5
#define RCLOWER 76.0
#define RCUPPER 146.0

class Switches
{
  public:
    Switches();
    void begin();
    uint8_t value();
    float dial_percent();
};

#endif
