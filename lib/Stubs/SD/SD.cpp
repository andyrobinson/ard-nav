#include "SD.h"

namespace SDLib {

  bool SDClass::begin(uint32_t clock, uint8_t csPin) { return true; }

  File SDClass::open(const char *filepath, uint8_t mode) {
      strcpy(readwrite_filepath,filepath);
      return File();
  }

  bool SDClass::exists(const char *filepath) {
    return true;
  }

  bool SDClass::remove(const char *filepath) {
    strcpy(remove_filepath, filepath);
    return true;
  }

  char *SDClass::last_remove() {
    return remove_filepath;
  }

  char *SDClass::last_filepath() {
    return readwrite_filepath;
  };

  char *SDClass::file_contents() {
    return contents;
  };


  void SDClass::reset() {
    strcpy(remove_filepath,"");
    strcpy(readwrite_filepath,"");
    strcpy(contents,"");
  }

  SDClass SDStub;

};
