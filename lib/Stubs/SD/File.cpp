#include "SD.h"

using namespace SDLib;

File::File(char* content_ptr) {
    file_buffer = content_ptr;
    buffer_index = 0;
};

uint16_t File::write(uint8_t val) {
    return write(&val, 1);
}

uint16_t File::write(const uint8_t *buf, uint16_t size) {
    for (int i=0; i < size; i++) {
        file_buffer[buffer_index++] = buf[i];
    }
    file_buffer[buffer_index]='\0';
    return 0;
};

void File::reset() {
    buffer_index = 0;
}

int File::availableForWrite() {
    return 1;
};

char* File::contents_as_string() {
    return file_buffer;
};

int File::read(){
    if (file_buffer[buffer_index] == '\0')
        return -1;
    else
        return file_buffer[buffer_index++];
};

int File::available(){
    return file_buffer[buffer_index] != '\0';
};


int File::peek(){return 0;};
void File::flush(){};
void File::close(){};
