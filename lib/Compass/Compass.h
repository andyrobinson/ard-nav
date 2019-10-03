#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20

struct MagResult {
  int x;
  int y;
  int z;
};

class Compass
{
  public:
    Compass();
    MagResult bearing();
    MagResult accel();
    void begin();

  private:
    void write8(byte address, byte reg, byte value);
    byte read8(byte address, byte reg);
    int hilow_toint(byte high, byte low);
};

#endif
