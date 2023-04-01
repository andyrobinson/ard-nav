#include "SD.h"

namespace SDLib {

  bool SDClass::begin(uint32_t clock, uint8_t csPin) { return true; }

  File SDClass::open(const char *filepath, uint8_t mode) {
      return File();
  }

  bool SDClass::exists(const char *filepath) {
    return true;
  }

  bool SDClass::remove(const char *filepath) {
    return true;
  }

  SDClass SD;

};
