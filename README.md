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

## Standards

* All rotary measurements are in degrees, either as integers (default) or doubles (only where needed)
* For calculations other than latitude/longitude to the nearest whole degree is sufficient
* All linear measurements are in metres (GPS speed should be converted to metres/sec)
* All time measurements are in milliseconds

## Current libraries

* Angle - utilities for dealing with angles and signed -180 to +180 (angle) and unsigned 0 - 359 (uangle) types.
* Compass - wrapper for the Adafruit LSM303DLHC compass module, with tilt adjustment
* Globe - lat/long calculations  
* Helm - Steer a course, either directly, or by tacking
* Position - lat/long struct with error values
* Rudder - wrapper around servo to limit deflection
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* TestEg - example of using Gtest (google C++ test library)
* Timer - wrapper around delay function, for testing and possible multi-tasking
* Utility - templates for common functions
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## In Progress
** Create a Helm that will steer a course, either directly or by executing a left/right tack
** Add OLED display again if possible (do we need special digital pins?) so that we can check the compass bias without USB power
** Adjust rudder if current deflection is not sufficient to turn or maintain course
** tacking

## Planned development

1. Create a wrapper around the GPS library to make it easy to use
2. Create a Waypoint follower that follows a set of waypoints, optionally repeating them
3. Create a Navigator that will navigate to a single position by asking the Helm to steer a course
4. Logging - requires the introduction of an SSD device
