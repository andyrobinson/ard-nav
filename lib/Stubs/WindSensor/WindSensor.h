
#ifndef WindSensorStub_h
#define WindSensorStub_h

#include "Angle.h"

using namespace Angle;

class WindSensor
{
  public:
    WindSensor();
    angle relative();
    uangle absolute(uangle bearing);
    void set_relative(angle *rel_angles, int size);

  private:
    int list_size;
    int current_position;
    angle *response_list;
};

#endif
