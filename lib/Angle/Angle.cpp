#include "Angle.h"

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

    uangle uclockwise(uangle start, uangle delta) {
      return (start + delta) % 360;
    }

    uangle uanticlock(uangle start, uangle delta) {
      return (start + 360 - delta) % 360;
    }

    uangle uadd(uangle start, angle delta) {
    return (start + 360 + delta) % 360;    }

}
