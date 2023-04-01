#ifndef SDStub_h
#define SDStub_h

#include "cstdio"
#include "cstring"

uint8_t const O_READ = 0X01;
uint8_t const O_WRITE = 0X02;
uint8_t const O_APPEND = 0X04;
uint8_t const O_CREAT = 0X10;

#define FILE_READ O_READ
#define FILE_WRITE (O_READ | O_WRITE | O_CREAT | O_APPEND)

namespace SDLib {

  class File {
    public:
      File(void);
      virtual uint16_t write(uint8_t);
      virtual uint16_t write(const uint8_t *buf, uint16_t size);
      virtual int availableForWrite();
      virtual int read();
      virtual int peek();
      virtual int available();
      virtual void flush();
      virtual void close();

      // using Print::write;
  };

  class SDClass {

    public:
      bool begin(uint32_t clock, uint8_t csPin);
      File open(const char *filename, uint8_t mode = FILE_READ);
      bool exists(const char *filepath);
      bool remove(const char *filepath);

      // for test purposes
      char *last_remove();
      char *last_filepath();
      char *file_contents();
      void reset();

     private:
       char remove_filepath[50];
       char readwrite_filepath[50];
       char contents[1000];

  };

  extern SDClass SDStub;

};

#endif
