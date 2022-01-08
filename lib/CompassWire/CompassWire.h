#ifndef CompassWire_h
#define CompassWire_h

#include "Arduino.h"
#include "Compass.h"
#include "Gps.h"

#define COMPASS_COMPASS_I2C_ADDRESS 0x1E
#define COMPASS_ACCEL_I2C_ADDRESS 0x19

#define COMPASS_REGISTER_ENABLE        0x02
#define COMPASS_REGISTER_X_HIGH        0x03
#define ACCEL_REGISTER_OUT_X_L_A       0x28
#define COMPASS_ACCEL_CTRL_REG1_A      0x20
#define COMPASS_X_CORRECTION           -100

class CompassWire : public Compass {
  public:
    CompassWire();
    uangle bearing();
    void begin();
    MagResult raw_bearing();
    MagResult raw_accel();
    int err_percent();

  private:
    int hilow_toint(byte high, byte low);
    void write8(byte address, byte reg, byte value);
    int errors;
};

#endif
