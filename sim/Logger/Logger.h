#ifndef LoggerSim_h
#define LoggerSim_h

#include <cstdio>  // Hacked in here to avoid library code changes ... ho hum

class Logger
{
  public:
    virtual void begin() = 0;
    virtual void msg(char *message) = 0;
    virtual void banner(char *message) = 0;
    virtual void setdest(char destletter) = 0;
    virtual void settack(char tackletter) = 0;
};

#endif
