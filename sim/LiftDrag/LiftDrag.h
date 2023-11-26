#ifndef LiftDrag_h
#define LiftDrag_h

#include "Angle.h"
#include "Utility.h"

#define A_LOW -0.004
#define B_LOW 0.2
#define C_LOW -0.9

#define X_OFFSET_HIGH -42
#define A_HIGH 0.000012
#define B_HIGH -0.00055
#define C_HIGH -0.029
#define D_HIGH 1.32

#define SAIL_AREA 0.14
#define AIR_DENSITY 1.29

using namespace Angle;
using namespace Utility;

namespace LiftDrag {
 double Cd(angle attack);
 double Cl(angle attack);
 double drag(angle attack, double wind_speed);
 double lift(angle attack, double wind_speed);
}

#endif
