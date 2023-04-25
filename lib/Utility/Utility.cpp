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

  void stuff(long val, uint8_t *buff, int start, int length) {
    long val2 = val;
    for (int i=start; (i - start) < length; i++) {
        buff[i] = val2 & 0xFF;
        val2 = val2 >> 8;
    }
  }

  void toHex(uint8_t *in, char *out, int length) {
    for (int i=0; i<length; i++) {
        out[i*2+1] = (in[i] & 0x2) + '0';
        out[i*2] = 7 + '0';
    }
  }

  extern "C" char* sbrk(int incr);
  int dispFreeMemory() {
    char top;
    return &top - reinterpret_cast<char*>(sbrk(0));
  }
}
