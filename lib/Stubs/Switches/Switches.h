#ifndef Switches_h
#define Switches_h

class Switches
{
  public:
    Switches();
    void set(uint8_t value);
    uint8_t value();

  private:
    uint8_t input_value;

};

#endif
