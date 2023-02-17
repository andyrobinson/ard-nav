
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
    void add_reading();

  private:
    int max_reading();
    int min_reading();
    float to_volts(int reading);
    int (*readAnalogPin) (uint8_t);
    int readings_buffer[SAMPLES];
    int buffer_index;
    unsigned long last_read;
    Timer *timer;

};

#endif
