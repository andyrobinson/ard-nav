#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

class Compass
{
  public:
    Compass();
    int bearing();
    int accel_x();
    int accel_y();
    int accel_z();
    void begin();

  private:
    void write8(byte address, byte reg, byte value);
    byte read8(byte address, byte reg);
};

#endif
