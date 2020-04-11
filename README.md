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

## In Progress

## Planned development

1. Investigate why the GPS time reported using serial logger does not vary from initial time
2. Rewrite functions which return structs to using pointer arguments, for efficiency and
more importantly memory conservation
3. Remove all reference to String (object) to conserve memory - mainly logging
4. Logging - requires the introduction of an SSD device
5. Need some kind of integration testing
6. Need applications based on Helm then Tacker so that we can field test these parts without
having to worry about the GPS
7.  What kind of error values does the GPS give you? Ideally want it in metres.  Unfortunately the device can only give you a value for how the satellite positions may affect the accuracy, not the absolute accuracy.  According to Adafruit the accuracy is no more than 1.8m radius, so we should probably take the PDOP value and multiply it by 2 to give an estimate of the error radius.  However we really need to check that this value is being correctly populated, as it depends on which sentences we are listening to, so need to look at actual values
8.  We need follow-on code for failed sensors - wind direction (can we find out by steering?), compass (use GPS), gps (use dead reckoning).  Maybe ultimately we need more than one sensor.
9.  Note that when the input voltage falls below 6v that spikes caused by servo operation cause the Arduino to crash.  We need to ensure that the Arduino power supply is protected, and there is a fall-back reset.  Ideally whe the batteries get low we shut down until they regain some charge (I guess this could be never ...)
