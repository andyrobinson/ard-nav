#include "LiftDrag.h"

namespace LiftDrag {
 double Cd(angle attack) {
    // simple quadratic, use big divisor to get shallow curve
    double at = (double) min1((short) 180,max1(attack,(short) 0));
    if (at > 90.0) at = 180.0 - at;
    return 0.2+((at*at)/7100.0);
 }

 double Cl(angle attack) {

    // we use two quadratics, (a.x^2 + b.x + c) one for up to max lift, one for afterwards
    double at = (double) attack;
    double result;
    if (at < 25.0001) {
        result = A_LOW * (at * at) + B_LOW * at + C_LOW;
    } else {
        result = A_HIGH * (at * at) + B_HIGH * at + C_HIGH;
    }
    if (result < 0.0) result = 0.0;

    return result;
 }
}