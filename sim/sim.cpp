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
#include "Routes.h"
#include "RotaryPID.h"

Globe globe;

Switches switches;
char logmsg[22];

// Dependency injection
position start_point = pfF.pos;
Boat boat(&start_point);
Timer timer(&boat);
WindSensor windsensor(&boat);
Compass compass(&boat);
Gps gps(&boat,&timer);
Sail sail(&boat);
Rudder rudder(&boat);
ConsoleLogger logger(&boat, &timer);
RotaryPID rotaryPID(RUDDER_MAX_DISPLACEMENT,&switches,&logger);
Helm helm(&rudder, &compass, &timer, &windsensor, &sail, &rotaryPID, &logger);
Tacker tacker(&helm, &compass, &windsensor, &logger);
Navigator navigator(&tacker, &gps, &globe, &logger);
Captain captain(&navigator);

int main() {
    switches.set(1); // simulation of switch
    switches.set_percent(50.0);

    std::cout << "Simulation starting ...\n\n";

    uint8_t sw = switches.value() & 3; // four routes configurable
    route journey = plattfields[sw];
    captain.voyage(journey.waypoints, journey.length);

    std::cout << "\nSimulation finished\n";
    return 0;
}
