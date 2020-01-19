#ifndef Logger_h
#define Logger_h

#include "Position.h"
#include "Angle.h"

using namespace Angle;
using namespace Position;

class Logger
{
  public:
    Logger();
    void begin();
    void info(position *current_position, angle wind, uangle bearing, String message);
};

#endif
