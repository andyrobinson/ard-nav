#ifndef Logger_h
#define Logger_h

class Logger
{
  public:
    virtual void begin() = 0;
    virtual void msg(const char *message) = 0;
    virtual void banner(const char *message) = 0;
    virtual void setdest(char destletter) = 0;
    virtual void settack(char tackletter) = 0;
    virtual void write_version(int major, int minor) = 0;
};

#endif
