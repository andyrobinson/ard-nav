#ifndef StubLogger_h
#define StubLogger_h

#include "Logger.h"

class StubLogger : public Logger {
    public:
      StubLogger();
      virtual void begin();
      virtual void msg(const char *message);
      virtual void banner(const char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);
      char *last_message();
      char waypoint;

    private:
      char last_msg[30];
};

#endif
