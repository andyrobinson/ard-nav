
#ifndef Battery_h
#define Battery_h

#include "Timer.h"
#include "Arduino.h"

#define LIPO1 A0
#define LIPO2 A1

#define MAX_ANALOG 1024.0  // default 10 bits
#define SAMPLES 20 // currently depends on being called - might be good to add this to the timer wait
#define READ_DELAY 1000

class Battery
{
  public:
    Battery();
    Battery(int (*analogPinFn)(uint8_t), Timer *timerp);
    float lipomaxv(uint8_t batt);
    float lipominv(uint8_t batt);
    uint16_t raw_max(uint8_t batt);
    uint16_t raw_min(uint8_t batt);
    void add_reading();

  private:
    float to_volts(uint16_t reading);
    int (*readAnalogPin) (uint8_t);
    uint16_t readings_buffer[2][SAMPLES];
    int buffer_index;
    unsigned long last_read;
    Timer *timer;

};

#endif
