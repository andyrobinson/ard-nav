#include "Angle.h"
#include "Utility.h"

using namespace Utility;

namespace Angle {
    angle clockwise(angle start, uangle delta) {
      return (start + 180 + delta) % 360 - 180;
    }

    angle anticlock(angle start, uangle delta) {
      return (start + 540 - delta) % 360 - 180;
    }

    angle add(angle start, angle delta) {
      angle result = ((start + 540 + delta) % 360) - 180;
      return (angle) result;
    }

    uangle to_uangle(angle a) {
      return (a >= 0) ? a : (360 + a);
    }

    uangle uclockwise(uangle start, uangle delta) {
      return (start + delta) % 360;
    }

    uangle uanticlock(uangle start, uangle delta) {
      return (start + 360 - delta) % 360;
    }

    uangle uadd(uangle start, angle delta) {
    return (start + 360 + delta) % 360;    }

    angle udiff(uangle a, uangle b) {
      angle result = b - a;
      if (abs1(result) < 180) return result;
      return result - (sign(result) * 360);
    }

    angle to_angle(uangle ua) {
      return (ua <= 180) ? ua : (ua - 360);
    }

    uangle clockwisediff(uangle start, uangle end) {
      return (360 + end - start) % 360;
    }

    bool in_range(uangle value, uangle lower, uangle upper) {
        return clockwisediff(lower,value) + clockwisediff(value,upper) < 360;
    }

    double to_radians(double degrees) {
        return degrees/DEGREES_IN_RADIAN;
    }

    double to_degrees(double radians) {
      return radians * DEGREES_IN_RADIAN;
    }

}
