# ard-nav
Self sailing using the Arduino zero.

## Structure
The libraries are contained in sub folders under lib.  This includes open source libraries (typically Adafruit libraries), and home grown ones.

The home grown libraries all have a make file which will compile the library and run the tests using the standard C++ compiler on Windows or Mac.  Unfortunately this is not a guarantee of correct functioning (or even compilation) for Arduino, which uses it's own compiler and libraries.

The other folders represent Arduino applications.  Again each folder has a makefile, but in this case it just copies the required libraries into the libraries sub-folder, where the Arduino compiler will look for the source files.  The .ino file is the Arduino main file which references the libraries.

## Build and test

* Libraries should be built using Make in the library folder.
* Arduino applications require a two stake process:
  1. Run make to copy the latest library source files
  2. Run the Arduino IDE, set the library folder to the correct one for the project, then build the application and upload it on to the board.

## Current libraries

* Angle - utilities for dealing with angles and signed -180 to +180 (angle) and unsigned 0 - 359 (uangle) types.
* Compass - wrapper for the Adafruit LSM303DLHC compass module, with tilt adjustment
* Globe - lat/long calculations  
* Position - lat/long struct with error values
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* TestEg - example of using Gtest (google C++ test library)
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## Planned development

1. Use the Arduino command line compiler to save having to use the IDE
2. Extend the compass library so that it does the tilt adjustment calculations.  This is very hard to test, so probably just create a test application (See the sandbox)
3. Create a Waypoint follower that follows a set of waypoints, optionally repeating them
4. Create a Navigator that will navigate to a single position by asking the Helm to steer a course
5. Create a Helm that will steer a course, either directly, or by executing a left/right tack
6. Create a Rudder that wraps the standard servo and limits rudder deflection to 45 degrees.
7. Logging - requires the introduction of an SSD device
