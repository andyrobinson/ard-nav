#ifndef LoggerStub_h
#define LoggerStub_h

#include "cstdio"

class Logger
{
  public:
    virtual void begin() = 0;
    virtual void msg(const char *message) = 0;
    virtual void banner(const char *message) = 0;
    virtual void setdest(char destletter) = 0;
    virtual void settack(char tackletter) = 0;
};

#endif
