
#ifndef Battery_h
#define Battery_h

#include "Timer.h"

#ifdef ARDUINO
#include "Arduino.h"
#define LIPO1 A0
#else
#define LIPO1 99
#endif

#define MAX_ANALOG 1024.0  // default 10 bits
#define SAMPLES 20 // currently depends on being called - might be good to add this to the timer wait
#define READ_DELAY 1000

class Battery
{
  public:
    Battery();
    Battery(int (*analogPinFn)(uint8_t), Timer *timerp);
    float lipo1maxv();
    float lipo1minv();
    uint16_t raw_max();
    uint16_t raw_min();
    void add_reading();

  private:
    float to_volts(uint16_t reading);
    int (*readAnalogPin) (uint8_t);
    uint16_t readings_buffer[SAMPLES];
    int buffer_index;
    unsigned long last_read;
    Timer *timer;

};

#endif
