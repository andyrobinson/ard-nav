#include <iostream>
#include "Boat.h"
#include "ConsoleLogger.h"
#include "Timer.h"
#include "Position.h"

using namespace Position;

position start_point = {53.44580, -2.22515, 3.0};
ConsoleLogger logger;
Boat boat(&start_point, &logger);
Timer timer(&boat);

int main() {
    char buffer[100];
    unsigned long tm = 0;
    position pos;

    std::cout << "Simulation starting ...\n\n";

    while (tm < 20000) {
      pos = boat.location();
      sprintf(buffer,"Lat %9.5f, Long %9.5f\n",pos.latitude, pos.longitude);
      logger.msg(buffer);
      tm += 2000;
      boat.rudder = 20;
      timer.wait(2000);
    }

    std::cout << "\nSimulation finished\n";
    return 0;
}
