#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#include "Angle.h"

#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20
#define COMPASS_X_CORRECTION           -100

using namespace Angle;

struct MagResult {
  int x;
  int y;
  int z;
};

class Compass
{
  public:
    Compass();
    uangle bearing();
    void begin();
    MagResult raw_bearing();
    MagResult raw_accel();
    int err_percent();

  private:
    int hilow_toint(byte high, byte low);
    bool wait_with_timeout(volatile bool *busy, int timeout);
    uint8_t data[6];
    int errors;
};

#endif
