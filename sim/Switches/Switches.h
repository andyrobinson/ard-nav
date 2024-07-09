#ifndef SwitchesSim_h
#define SwitchesSim_h

#include <stdint.h>

class Switches
{
  public:
    Switches();
    void set(uint8_t value);
    uint8_t value();
    void set_percent(float value);
    float dial_percent();


  private:
    uint8_t input_value;
    float percent;

};

#endif
