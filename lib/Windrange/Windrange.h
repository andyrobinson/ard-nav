#ifndef Windrange_h
#define Windrange_h

#include "Angle.h"

using namespace Angle;

namespace Windrange {

  typedef struct {
     angle port;
     angle starboard;
  } windrange;

inline bool operator==(const windrange& lhs, const windrange& rhs) {
    return (
      lhs.port == rhs.port &&
      lhs.starboard == rhs.starboard);
}

}
#endif