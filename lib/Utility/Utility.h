#ifndef Utility_h
#define Utility_h

namespace Utility {
  template <typename T> T sign(T value) {
   return T((value>0)-(value<0));
  }

  template <typename T> T abs1(T value) {
   return value < 0 ? -value : value;
  }
}

#endif
