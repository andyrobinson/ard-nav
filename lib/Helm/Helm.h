#ifndef Helm_h
#define Helm_h

#include "IHelm.h"
#include "Rudder.h"
#include "Compass.h"
#include "Timer.h"
#include "WindSensor.h"
#include "Sail.h"
#include "Logger.h"
#include "RotaryPID.h"
#include "Windrange.h"
#include "Arduino.h"
#include "SatComm.h"

using namespace Windrange;

#define STEER_INTERVAL 200
#define MAX_OUT_OF_RANGE 30

class Helm : public IHelm
{
  public:
    Helm();
    Helm(Rudder *rudderp, Compass *compassp, Timer *timerp, WindSensor *windsensorp, Sail *sailp, RotaryPID *rotarypidp, SatComm *satcommp, Logger *loggerp);
    virtual void steer(uangle direction_param, long steer_time_param, windrange range_param);
    virtual bool steer_and_continue();

  private:
    Rudder *rudder;
    Compass *compass;
    Timer *timer;
    WindSensor *windsensor;
    Sail *sail;
    RotaryPID *rotarypid;
    SatComm *satcomm;
    Logger *logger;
    angle rudder_position;
    int out_of_range_count;
    long remaining;
    char plus[2] = "+";
    char logmsg[50];
    uangle direction;
    long steer_time;
    windrange range;


    bool ok_to_continue(long time_left, long total_time, windrange range);
};

#endif
