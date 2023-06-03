#ifndef Battery_Stub_h
#define Battery_Stub_h

#define MAX_ANALOG 1024.0  // default 10 bits

class Battery
{
  public:
    Battery();
    float lipo1maxv();
    float lipo1minv();
    uint16_t raw_max();
    uint16_t raw_min();
    void add_reading();

    // sensible values are in the range 3.3v - 4.2v, which translates to 500-650
    void setMaxMin(uint16_t max, uint16_t min);

  private:
    float to_volts(uint16_t reading);
    uint16_t rmax, rmin;

};

#endif
