#ifndef ConsoleLogger_h
#define ConsoleLogger_h

#include "Logger.h"

class ConsoleLogger : public Logger {
    public:
      ConsoleLogger();
      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      char destination;
      char tack;
};

#endif
