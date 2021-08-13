#include <iostream>
#include "Boat.h"
#include "ConsoleLogger.h"
#include "Timer.h"
#include "Position.h"

using namespace Position;

int main() {
    std::cout << "Simulation starting ...\n\n";
    position start_point = {53.44580, -2.22515, 3.0};
    char buffer[100];
    ConsoleLogger logger;
    Boat boat(&start_point, &logger);
    Timer timer(&boat);
    unsigned long tm = 0;
    position pos;

    while (tm < 10000) {
      pos = boat.location();
      sprintf(buffer,"Lat %9.5f, Long %9.5f\n",pos.latitude, pos.longitude);
      logger.msg(buffer);
      tm += 2000;
      boat.move(2000);
    }

    std::cout << "\nSimulation finished\n";
    return 0;
}
