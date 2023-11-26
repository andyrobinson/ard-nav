#include "LiftDrag.h"

namespace LiftDrag {
 double Cd(angle attack) {
    // simple quadratic, use big divisor to get shallow curve
    double at = (double) min1((short) 180,max1(attack,(short) 0));
    if (at > 90.0) at = 180.0 - at;
    return 0.2+((at*at)/7100.0);
 }

 double Cl(angle attack) {
    // we use a quadratic initially, then a cubic, (a.x^2 + b.x + c) one for up to max lift, one for afterwards
    double at = (double) attack;
    double result;
    if (at < 25.0001) {
        result = A_LOW * (at * at) + B_LOW * at + C_LOW;
    } else if (at < 90.0) {
        double x = at + X_OFFSET_HIGH;
        result = A_HIGH * (x * x * x) + B_HIGH * (x * x) + C_HIGH * x + D_HIGH;
    }
    else result = 0.0;

    if (result < 0.0) result = 0.0;

    return result;
 }

// lift and drag
// f = C * 0.5 * p * v ^2 * A
// p = density of air = 1.29
// v = wind speed m/s
// A = area of sail
// C is different for lift and drag, and varies with angle of attack
  double liftdrag(double coefficient, double wind_speed) {
    return 0.5 * coefficient * SAIL_AREA * AIR_DENSITY * (wind_speed * wind_speed);
  }

  double drag(angle attack, double wind_speed) {
    return liftdrag(Cd(abs1(attack)),wind_speed);
  }

  double lift(angle attack, double wind_speed) {
    return liftdrag(Cl(abs1(attack)),wind_speed);
  }

}