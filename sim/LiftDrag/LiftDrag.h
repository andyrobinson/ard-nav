#ifndef LiftDrag_h
#define LiftDrag_h

#include "Angle.h"
#include "Utility.h"

#define A_LOW -0.004
#define B_LOW 0.2
#define C_LOW -0.9

#define A_HIGH -0.0002844
#define B_HIGH 0.01422
#define C_HIGH 1.422

#define SAIL_AREA 0.14
#define AIR_DENSITY 1.29

using namespace Angle;
using namespace Utility;

namespace LiftDrag {
 double Cd(angle attack);
 double Cl(angle attack);
 double drag(angle attack, double wind_speed);
 double lift(angle attack, double wind_speed);
// double liftdrag(double coefficient, double wind_speed);
}

#endif
