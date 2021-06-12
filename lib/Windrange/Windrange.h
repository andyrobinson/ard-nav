#ifndef Windrange_h
#define Windrange_h

#include "Angle.h"

using namespace Angle;

namespace Windrange {

  typedef struct {
     uangle lower;
     uangle upper;
  } windrange;

inline bool operator==(const windrange& lhs, const windrange& rhs) {
    return (
      lhs.lower == rhs.lower &&
      lhs.upper == rhs.upper);
}

}
#endif