
#include "SD.h"

using namespace SDLib;

File::File(void) {};
uint16_t File::write(uint8_t) {return 0;};
uint16_t File::write(const uint8_t *buf, uint16_t size) {return 0;};
int File::availableForWrite() {return 0;};
int File::read(){return 0;};
int File::peek(){return 0;};
int File::available(){return 0;};
void File::flush(){};
