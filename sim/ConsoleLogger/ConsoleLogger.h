#ifndef ConsoleLogger_h
#define ConsoleLogger_h

#include "Logger.h"
#include "Boat.h"
#include "Timer.h"

class ConsoleLogger : public Logger {
    public:
      ConsoleLogger();
      ConsoleLogger(Boat *boatp, Timer *timerp);

      virtual void begin();
      virtual void msg(char *message);
      virtual void banner(char *message);
      virtual void setdest(char destletter);
      virtual void settack(char tackletter);

    private:
      Boat *boat;
      Timer *timer;
      char destination;
      char tack;
};

#endif
