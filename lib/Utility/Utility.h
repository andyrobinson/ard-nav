#ifndef Utility_h
#define Utility_h

#ifdef ARDUINO
#include "Arduino.h"
#endif
#include "time.h"

#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

namespace Utility {
  template <typename T> T sign(T value) {
   return T((value>0)-(value<0));
  }

  template <typename T> T abs1(T value) {
   return value < 0 ? -value : value;
  }

  template <typename T> T min1(T value1, T value2) {
   return value1 < value2 ? value1 : value2;
  }

  template <typename T> T max1(T value1, T value2) {
   return value1 > value2 ? value1 : value2;
  }

  long unix_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t second);
  int dispFreeMemory();
}

#endif
