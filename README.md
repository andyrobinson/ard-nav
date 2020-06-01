# ard-nav
Self sailing using the Arduino zero.

## Structure
The libraries are contained in sub folders under lib.  This includes open source libraries (typically Adafruit libraries), and home grown ones.

The home grown libraries all have a make file which will compile the library and run the tests using the standard C++ compiler on Windows or Mac.  Unfortunately this is not a guarantee of correct functioning (or even compilation) for Arduino, which uses it's own compiler and libraries.

The other folders represent Arduino applications.  Again each folder has a makefile, in this case it copies the required libraries into the libraries sub-folder, where the Arduino compiler will look for the source files.  This has been extended so that compile and upload can be run directly from the Makefile.  The .ino file is the Arduino main file which references the libraries.

## Build and test

* Libraries should be built using Make in the library folder.
* Arduino applications should also be built using Make (currently Windows only)
  1. Make compile just does compile
  2. Make upload does compile and upload to the Arduino

## Standards

* All rotary measurements are in degrees, either as integers (default) or doubles (only where needed)
* For calculations other than latitude/longitude to the nearest whole degree is sufficient
* All linear measurements are in metres
* All time measurements are in milliseconds
* Speed is metres/sec (although this could be millimetres/millisecond!)

## Current libraries

* Angle - utilities for dealing with angles and signed -180 to +180 (angle) and unsigned 0 - 359 (uangle) types.
* Captain - follows a route consisting of a number of waypoints, using Navigator
* Compass - wrapper for the Adafruit LSM303DLHC compass module, with tilt adjustment
* DisplayLogger - logs to the OLED display
* Globe - lat/long calculations  
* Gps - wrapper for the Adafruit GPS Library which enables/disables the GPS, and gets a reading within a specified time (if possible)
* Helm - Steer a direct course and adjust the sail
* Logger - Abstract class for logging (implemented by specific loggers)
* MultiLogger - logs simultaneously to the given list of loggers
* Navigator - Navigate to a destination lat/long by repeatedly steering
* Position - lat/long struct with error values
* Rudder - wrapper around servo to limit deflection
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* SDLogger - logs to an SD card
* SelfTest - power-on self test routines
* SerialLogger - log to the serial port (for Arduino Serial monitor)
* Stubs - home grown stubs for use in unit testing
* Tacker - steer directly or do a tack, using Helm
* TestEg - example of using Gtest (google C++ test library)
* Timer - wrapper around delay function, for testing and possible multi-tasking
* Utility - templates for common functions, unix time
* Waypoint - waypoint definition
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## Current concerns

We probably need a turning mode and a steady state mode, and move between the modes based on deflection from course.

Display refresh causes jitter, but we can live with that for the moment (won't be an issue OTW unless SD card writing has the same problem).

The steering is jittery at the moment.  The sample rate of 1/10th of a second means that quite large rates of turn can be calculated (minimum is 10 degrees/sec), many just due to the inaccuracy of the compass.  It may be better to reduce the sample rate or do a low pass filter (i.e. average over 2 or 3 readings), but this will make it less responsive.

## Observations from field tests

### 13 May, Land based
* The rudder seems to make for weaving navigation - in part due to the unresponsiveness of a human vessel
* We need a better estimate of speed (and therefore time to destination).  If we underestimate the speed then we will overshoot the target.  Points towards moving averages for both speed and absolute wind direction (and therefore a regular update to both, probably via the GPS interrupt timer)
* the logging could be better - where are we heading, how long before next review, are we tacking, and is it tack1 or tack2?
* compass is much better outside
* broadly speaking the software appears to work(!)


## Planned development

1. Should introduce a deliberate memory leak to check that the memory measure works
1a.  Should log the memory usage
2. Need better indication from selftest that everything is well, so that we can be sure that nav has started when we no longer have the display
3. It would be good to use the analog inputs to measure the battery voltage.  Note that once we have a proper power supply this will require a connection directly from the battery
4. In the final attempt we need to disable self test (probably completely), to take account of the fact that the system may restart during navigation.  Also the navigation needs to identify the nearest waypoint when it starts, and determine if it has passed this waypoint before selecting a waypoint to navigate towards.
5. Compass is highly influenced by electric motors - need to check in-situ and potentially isolate - much better outdoors, still perhaps 5 degrees out in the box, so we need to check in the boat
6. Rewrite functions which return structs to using pointer arguments, for efficiency and
more importantly memory conservation
7. Need some kind of integration testing
8.  We need follow-on code for failed sensors - wind direction (can we find out by steering?), compass (use GPS), gps (use dead reckoning).  Maybe ultimately we need more than one sensor.
9.  Note that when the input voltage falls below 6v that spikes caused by servo operation cause the Arduino to crash.  We need to ensure that the Arduino power supply is protected, and there is a fall-back reset.  Ideally when the batteries get low we shut down until they regain some charge (I guess this could be never ...).  There are voltage regulators that will ensure a stable voltage for as long as possible.
10.  Don't forget fallback timer which reboots the arduino after a period of inactivity (aka crash)
11.  Need to review all limits (e.g. max steer time) before attempting longer journeys

## Hot tips

Looking at the serial output on a Mac (from https://stackoverflow.com/questions/12254378/how-to-find-the-serial-port-number-on-mac-os-x)

ls /dev/tty.*

then you can read that serial port using the screen command:

screen /dev/tty.[yourSerialPortName] [yourBaudRate]


## Notes on how Adafruit GPS Library works

The read() method just reads a single character.  It has two buffers, which it fills alternately.  When read() gets an end of line from the GPS, then it closes the current buffer with a null, sets a flag to say that there's data available and switches processing to the other buffer.   Note that there's no requirement to pick up the data, it just carries on alternating buffers.

This does mean that if there is data available (and we pause reading new characters from the GPS while we read it) then we have a stable set of readings.  Note also that there is a pause function for this purpose, although presumably we don't want to pause for very long.

When we attempt to parse the data using the call AGPS.parse(AGPS.lastNMEA()) then this resets the data available flag (call the newNMEAreceived() method) until a new GPS sentence is available.  Note also that parsing can fail, notably when the checksum doesn't work.

So it would appear that in the interrupt we just call the read() method.  When we want data from the GPS, we first pause reading, then if there is data available, we attempt to parse it and return the result.  If there is no data available or the parsing fails, then we should just return some kind of "NO FIX" result.  We should make sure that we unpause the GPS before we finish, regardless of the result.   We could do this in a loop so that we can wait for the GPS should we wish to.
