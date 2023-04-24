#include "Utility.h"

namespace Utility {
  long unix_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t second) {
  struct tm now;

  now.tm_year = year + 100;
  now.tm_mon = month-1; // zero based months
  now.tm_mday = day;
  now.tm_hour = hour;
  now.tm_min = min;
  now.tm_sec = second;
  now.tm_isdst = 0;

  return mktime(&now);
}

  extern "C" char* sbrk(int incr);
  int dispFreeMemory() {
    char top;
    return &top - reinterpret_cast<char*>(sbrk(0));
  }
}
