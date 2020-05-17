#ifndef StubLogger_h
#define StubLogger_h

#include "Logger.h"

class SDLogger : public Logger {
    public:
      StubLogger();
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);
};

#endif
