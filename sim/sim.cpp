#include <iostream>
#include "Boat.h"
#include "ConsoleLogger.h"

// TODO
#include "WindSensor.h"

// SIMULATION STUBS
#include "Timer.h"
#include "Rudder.h"
#include "Gps.h"
#include "Compass.h"
#include "Sail.h"
#include "Switches.h"

//NEUTRAL
#include "Position.h"
#include "Globe.h"
#include "Helm.h"
#include "Tacker.h"
#include "Navigator.h"
#include "Captain.h"
#include "Utility.h"
#include "Routes.h""
#include "RotaryPID.h"

Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
position start_point = pfF.pos;
ConsoleLogger logger;
Boat boat(&start_point, &logger);
WindSensor windsensor(&boat);
Compass compass(&boat);
Timer timer(&boat);
Gps gps(&boat,&timer);
Sail sail(&boat);
Rudder rudder(&boat);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

int main() {
    char buffer[100];
    unsigned long tm = 0;
    position pos;
    switches.set(0); // simulation of switch
    switches.set_percent(50.0);

    std::cout << "Simulation starting ...\n\n";

    while (tm < 20000) {
      sprintf(logmsg, "Switches %3d", switches.value()); logger.banner(logmsg);

      uint8_t sw = switches.value() & 3; // four routes configurable
      route journey = plattfields[sw];

      captain.voyage(journey.waypoints, journey.length);
    }

    std::cout << "\nSimulation finished\n";
    return 0;
}
