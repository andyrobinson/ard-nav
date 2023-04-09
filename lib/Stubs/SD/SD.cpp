#include "SD.h"

namespace SDLib {

  bool SDClass::begin(uint32_t clock, uint8_t csPin) { return true; }

  File SDClass::open(const char *filepath, uint8_t mode) {
      strcpy(readwrite_filepath,filepath);
      return File(file_chars);
  }

  bool SDClass::exists(const char *filepath) {
    return strcmp(filepath, existing_file) == 0;
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

  void SDClass::setup_file(char* filepath, char* initial_content) {
    strcpy(existing_file, filepath);
    strcpy(file_chars, initial_content);
  }

  char *SDClass::file_contents() {
    return file_chars;
  };

  void SDClass::reset() {
    strcpy(remove_filepath,"");
    strcpy(readwrite_filepath,"");
    strcpy(existing_file,"");
    strcpy(file_chars,"\0");
  }

  SDClass SDStub;

};
