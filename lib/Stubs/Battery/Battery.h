#ifndef Battery_Stub_h
#define Battery_Stub_h

#define MAX_ANALOG 1024.0  // default 10 bits

class Battery
{
  public:0
    Battery();
    float lipomaxv(uint8_t batt);
    float lipominv(uint8_t batt);
    uint16_t raw_max(uint8_t batt);
    uint16_t raw_min(uint8_t batt);
    void add_reading();

    // sensible values are in the range 3.3v - 4.2v, which translates to 500-650
    void setMaxMin(uint8_t batt, uint16_t max, uint16_t min);

  private:
    float to_volts(uint16_t reading);
    uint16_t rmax[2], rmin[2];

};

#endif
