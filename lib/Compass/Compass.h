#ifndef Compass_h
#define Compass_h

#include "Arduino.h"
#include "Angle.h"
#include "Timer.h"
#include "Utility.h"
#include "I2C.h"
#include "math.h"

#define LIS2MDL_ADDRESS     0x1E
#define LIS2MDL_CFG_REG_A   0x60
#define LIS2MDL_CFG_REG_C   0x62
#define LIS2MDL_OUTX_L_REG  0x68
#define LIS2MDL_STATUS_REG  0x67

#define COMPASS_ACCEL_I2C_ADDRESS      0x19
#define COMPASS_ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20

#define COMPASS_CACHE_TTL_MS            10
#define COMPASS_POWER_PIN               13
#define COMPASS_INITIAL_RESET_PAUSE_MS  100
#define COMPASS_MAX_RESET_PAUSE_MS     2000
#define COMPASS_RESET_ERROR_THRESHOLD  75
#define COMPASS_SECONDS_PER_HOUR       3600l
#define COMPASS_MAX_RESETS             3600l
#define COMPASS_INITIAL_PAUSE          50

#define COMPASS_X_CORRECTION           -70
#define COMPASS_Y_CORRECTION           -80
#define COMPASS_Z_CORRECTION           300

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
    int hilow_toint(uint8_t high, uint8_t low);
    Timer *timer;
    I2C *i2c;
    uangle tiltadjust;
    long last_read_time;
    long reset_pause;
    long reset_count;
    long reset_start;

};

#endif
