# ard-nav
Self sailing using the Arduino zero. See the [blog](https://noahs-barque.blogspot.com/p/blog-page.html) for more grisly details.

## Structure
The libraries are contained in sub folders under lib.  This includes open source libraries (typically Adafruit libraries), and home grown ones.

The home grown libraries all have a make file which will compile the library and run the tests using the standard C++ compiler on Windows or Mac.  Unfortunately this is not a guarantee of correct functioning (or even compilation) for Arduino, which uses it's own compiler and libraries.

The other folders represent Arduino applications.  Again each folder has a makefile, in this case it copies the required libraries into the libraries sub-folder, where the Arduino compiler will look for the source files.  This has been extended so that compile and upload can be run directly from the Makefile.  The .ino file is the Arduino main file which references the libraries.

## Hardware

This project is built around specific hardware, although it could be modified for other hardware.  There is a library component for each hardware
module, so it should be fairly straightfoward to replace the libraries as needed.  Current list of hardware:

* Arduino Zero (https://store.arduino.cc/arduino-zero)
* GPS (Adafruit Ultimate GPS - https://www.adafruit.com/product/746)
* Compass/Accelerometer (LSM303DLHC, Adafruit breakout board https://www.adafruit.com/product/1120)
* Wind direction sensor (AMS AS5048B https://ams.com/AS5048B)
* Pololu Micro Maestro servo controller (https://www.pololu.com/product/1350)
* Adafruit USB/DC/Solar Lipo charger (bq24074 https://www.adafruit.com/product/4755)
* Two servos, one for the sail, one for the rudder

Other hardware is optional:
* OLED display for initial debugging
* SD module for data logging
* Switches/RC input for changing configuration without reprogramming

## Build and test

* Libraries should be built using Make in the library folder.
* Arduino applications should also be built using Make (currently Windows only)
  1. Make compile just does compile
  2. Make upload does compile and upload to the Arduino

## Standards

* All rotary measurements are in degrees, either as integers (default) or doubles (only where needed) - see the uangle and angle types below
* Clockwise increments are always positive, Anticlockwise are negative (sail, rudder, wind direction)
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
* RotaryPID - implementation of a PID (proportional-integral-derivative) algorithm for steering a course
* Routes - contains courses to steer
* Rudder - wrapper around servo to limit deflection
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* SDLogger - logs to an SD card
* SelfTest - power-on self test routines
* SerialLogger - log to the serial port (for Arduino Serial monitor)
* Stubs - home grown stubs for use in unit testing
* Switches - reads three switches and a proportional RC input for in-the-field configuration changes
* Tacker - steer directly or do a tack, using Helm
* TestEg - example of using Gtest (google C++ test library)
* Timer - wrapper around delay function, for testing and possible multi-tasking
* Utility - templates for common functions, unix time
* Waypoint - waypoint definition
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## Current concerns

1. Voltage dips (and possibly spikes) caused by startup current of electric motors.  Ideas to reduce:
- Use a smooth turning servo Library to avoid big jumps
- The likely reason this is more of a problem OTW is that the water offers more resistance to the servo, hence bigger current draw - again fixed by smooth turning
- reduce power of rudder servo
- do not move both rudder and sail at the same time (not ideal)

2. This is an ideal time to write and check watchdog Timer!

3. Introduction of solar power is likely to disrupt refinement of steering and navigation software

## Short term plan

1.  Refine the navigation and steering.  Broadly speaking it appears to work, but needs a bit of tweaking.  The quick way to do this is to use
two separate batteries, which given that we will probably be splitting out the power supplies is a good thing.  May do one more session before
taking everything apart to implement solar power
2.  Change the RC switch wiring so that by default it will be computer controller, even if the RC transmitter is off
3.  Add solar power - this is a bigger piece of work involving removal of the deck.  At the same time:
  * Investigate 5v power options for Arduino - USB?
  * Fit thermistor to charging circuit
  * Check if the battery cases will fit into the keel space (but don't leave it down there for the moment)
  * Add suppression ceramic capacitors to both motors
  * Replace the 20Kg/cm servo with a 10Kg/cm servo on the rudder

## Observations from field tests

See the blog (https://noahs-barque.blogspot.com/p/blog-page.html) for field reports

## Planned development (longer term notes)

1. Should introduce a deliberate memory leak to check that the memory measure works
2. It would be good to use the analog inputs to measure the battery voltage.  Note that once we have a proper power supply this will require a connection directly from the battery
3. In the final attempt we need to disable self test (probably completely), to take account of the fact that the system may restart during navigation. It may be enough just to know that the GPS has got signal - clear window on the main module?  
4. Also the navigation needs to identify the nearest waypoint when it starts, and determine if it has passed this waypoint before selecting a waypoint to navigate towards.
5. Need some kind of integration testing
6.  We need follow-on code for failed sensors - wind direction (can we find out by steering?), compass (use GPS), gps (use dead reckoning).  Maybe ultimately we need more than one sensor.
7.  There are definitely problems with power fluctuation, although probably not spikes.  The current servo draws a lot of current - will replace with a less powerful version, and also reduce the voltage to the electric motors.  The test rig works fine running directly from the solar charging output (around 4v)
8.  Don't forget fallback (watchdog) timer which reboots the arduino after a period of inactivity (aka crash)
9.  Need to review all limits (e.g. max steer time) before attempting longer journeys
10. Other checks to Consider
- in irons
- becalmed (ensure preservation of battery)
- in a storm (ditto)

## Done (for reference)

- Logging now includes memory
- Need better indication from selftest that everything is well, so that we can be sure that nav has started when we no longer have the display
- Rewrite functions which return structs to using pointer arguments, for efficiency and
more importantly memory conservation
- software versioning
- average speed
- abandon current helm session if wind veers out of an expected range (currently too tight when tacking)

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
