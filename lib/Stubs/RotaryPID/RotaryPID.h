#ifndef RotaryPIDStub_h
#define RotaryPIDStub_h

#include "Utility.h"
#include "Angle.h"

using namespace Angle;

class RotaryPID
{
  public:
      RotaryPID();
      angle calculate(uangle desired_heading, uangle current_heading, long interval_ms);
      void set_results(angle *results, int size);

  private:
      angle *result_list;
      int current_position;
      int list_size;
};

#endif
