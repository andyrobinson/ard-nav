#ifndef WindSensorWire_h
#define WindSensorWire_h

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "Wire.h"
#include "WindSensor.h"

#define WINDSENSOR_AS5048B_I2C_ADDRESS 0x40
#define WINDSENSOR_AS5048B_I2C_REGISTER 0xFE

class WindSensorWire: public WindSensor {
  public:
    WindSensorWire();
    angle relative();
    uangle absolute(uangle bearing);
    void begin();
        int err_percent();
  private:
        int errors;
};

#endif
