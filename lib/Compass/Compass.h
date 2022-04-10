#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#include "Angle.h"
#include "Timer.h"

#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20
#define COMPASS_X_CORRECTION           -100
#define COMPASS_CACHE_TTL_MS            10
#define COMPASS_POWER_PIN               13
#define COMPASS_RESET_PAUSE_MS        100

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
    Compass(Timer *timerp);
    void begin();
    uangle bearing();
    MagResult raw_bearing();
    MagResult raw_accel();
    int err_percent();

  private:
    void reset();
    int hilow_toint(byte high, byte low);
    void write8(byte address, byte reg, byte value);
    Timer *timer;
    uangle tiltadjust;
    long last_read_time;
    int errors;

};

#endif
