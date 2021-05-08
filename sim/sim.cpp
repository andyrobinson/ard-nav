#include <iostream>
#include "Boat.h"
#include "ConsoleLogger.h"

int main() {
    std::cout << "Simulation starting ...\n\n";
    char buffer[100];
    ConsoleLogger logger;
    Boat boat(&logger);

    position pos = boat.location();
    sprintf(buffer,"Lat %9.5f, Long %9.5f",pos.latitude, pos.longitude);
    logger.banner(buffer);

    std::cout << "Simulation finished\n";
    return 0;
}