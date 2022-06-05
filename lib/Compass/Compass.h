#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#include "Angle.h"
#include "Timer.h"
#include "Utility.h"
#include "I2C.h"

#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20
#define COMPASS_X_CORRECTION           -100
#define COMPASS_CACHE_TTL_MS            10
#define COMPASS_POWER_PIN               13
#define COMPASS_INITIAL_RESET_PAUSE_MS  100
#define COMPASS_MAX_RESET_PAUSE_MS     2000
#define COMPASS_RESET_ERROR_THRESHOLD  75
#define COMPASS_SECONDS_PER_HOUR       3600
#define COMPASS_MAX_RESETS             3600

using namespace Angle;
using namespace Utility;

struct MagResult {
  int x;
  int y;
  int z;
  bool valid;
};

class Compass
{
  public:
    Compass();
    Compass(I2C *i2cp, Timer *timerp);
    void begin();
    uangle bearing();
    MagResult raw_bearing();
    MagResult raw_accel();
    int err_percent();
    long resets_per_hour();

  private:
    void reset();
    int hilow_toint(byte high, byte low);
    void write8(byte address, byte reg, byte value);
    Timer *timer;
    I2C *i2c;
    uangle tiltadjust;
    long last_read_time;
    long reset_pause;
    long reset_count;
    long reset_start;

};

#endif
