#ifndef Battery_h
#define Battery_h

#include "Arduino.h"

#define LIPO1 A0
#define MAX_ANALOG 1024.0  // default 10 bits
#define SAMPLES 30 // currently depends on being called - might be good to add this to the timer wait

class Battery
{
  public:
    Battery();
    float lipo1v();
    float lipo1avgv();

  private:
    float average1;
    int readings1;
    void add_reading(float battery_volts);
    float read_voltage();

};

#endif
