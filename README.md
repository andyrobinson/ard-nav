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
* Compass - wrapper for the Adafruit LSM303DLHC compass module, with tilt adjustment
* DisplayLogger - logs to the OLED display
* Globe - lat/long calculations  
* Gps - wrapper for the Adafruit GPS Library which enables/disables the GPS, and gets a reading within a specified time (if possible)
* Helm - Steer a direct course and adjust the sail
* Navigator - Navigate to a destination lat/long by repeatedly steering
* Position - lat/long struct with error values
* Rudder - wrapper around servo to limit deflection
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* SerialLogger - log to the serial port (for Arduino Serial monitor)
* Stubs - home grown stubs for use in unit testing
* Tacker - steer directly or do a tack, using Helm
* TestEg - example of using Gtest (google C++ test library)
* Timer - wrapper around delay function, for testing and possible multi-tasking
* Utility - templates for common functions, unix time
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## Current concerns

* System is much improved by recent changes but steering needs sorting out:
** System does occasionally reset to neutral position then go back to maximum deflection
** It does not wait long enough before increasing the deflection, but this could be sorted by slower refresh
** When turning the rudder skips back to central position, rather than tracking the distance from the desired allocation
in a progressive way.
** we probably need a turning mode and a steady state mode, and move between the modes based on deflection from course
** display refresh causes jitter, but we can live with that for the moment (won't be an issue OTW unless SD card writing has the same problem)

* The compass appears to have drifted - needs checking

## Planned development

1. Compass is highly influence by electric motors - need to check in-situ and potentially isolate
2. Rewrite functions which return structs to using pointer arguments, for efficiency and
more importantly memory conservation
3. Remove all reference to String (object) to conserve memory - mainly logging
4. Logging - requires the introduction of an SSD device
5. Need some kind of integration testing
6.  What kind of error values does the GPS give you? Ideally want it in metres.  Unfortunately the device can only give you a value for how the satellite positions may affect the accuracy, not the absolute accuracy.  According to Adafruit the accuracy is no more than 1.8m radius, so we should probably take the PDOP value and multiply it by 2 to give an estimate of the error radius.  However we really need to check that this value is being correctly populated, as it depends on which sentences we are listening to, so need to look at actual values
7.  We need follow-on code for failed sensors - wind direction (can we find out by steering?), compass (use GPS), gps (use dead reckoning).  Maybe ultimately we need more than one sensor.
8.  Note that when the input voltage falls below 6v that spikes caused by servo operation cause the Arduino to crash.  We need to ensure that the Arduino power supply is protected, and there is a fall-back reset.  Ideally whe the batteries get low we shut down until they regain some charge (I guess this could be never ...)
9.  Don't forget fallback timer which reboots the arduino after a period of inactivity (aka crash)
10.  Need to review all limits (e.g. max steer time) before attempting longer journeys

## Notes on how Adafruit GPS Library works

The read() method just reads a single character.  It has two buffers, which it fills alternately.  When read() gets an end of line from the GPS, then it closes the current buffer with a null, sets a flag to say that there's data available and switches processing to the other buffer.   Note that there's no requirement to pick up the data, it just carries on alternating buffers.

This does mean that if there is data available (and we pause reading new characters from the GPS while we read it) then we have a stable set of readings.  Note also that there is a pause function for this purpose, although presumably we don't want to pause for very long.

When we attempt to parse the data using the call AGPS.parse(AGPS.lastNMEA()) then this resets the data available flag (call the newNMEAreceived() method) until a new GPS sentence is available.  Note also that parsing can fail, notably when the checksum doesn't work.

So it would appear that in the interrupt we just call the read() method.  When we want data from the GPS, we first pause reading, then if there is data available, we attempt to parse it and return the result.  If there is no data available or the parsing fails, then we should just return some kind of "NO FIX" result.  We should make sure that we unpause the GPS before we finish, regardless of the result.   We could do this in a loop so that we can wait for the GPS should we wish to.
