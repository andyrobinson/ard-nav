# ard-nav
Self sailing using the Arduino zero.  See the [blog](https://noahs-barque.blogspot.com/p/blog-page.html) for more grisly details

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

* All rotary measurements are in degrees, either as integers (default) or doubles (only where needed) - see the uangle and angle types below
* Clockwise increments are always positive, Anticlockwise are negative (sail, rudder, wind direction)
* For calculations other than latitude/longitude to the nearest whole degree is sufficient
* All linear measurements are in metres
* All time measurements are in milliseconds
* Speed is metres/sec (although this could be millimetres/millisecond!)

## External libraries
* Adafruit_GFX - graphics library for OLED display
* Adafruit_GPS_Library - for GPS
* Adafruit_SleepyDog_Library - provides sleep and watchdog functionality
* Adafruit_SSD1306 - library for OLED display
* PololuMaestro - servo controller

## Current libraries

* Angle - utilities for dealing with angles and signed -180 to +180 (angle) and unsigned 0 - 359 (uangle) types.
* Battery - collects voltage readings and returns max and min values (assumes single LIPO with voltage halved)
* Captain - follows a route consisting of a number of waypoints, using Navigator
* Compass - wrapper for the Adafruit LSM303DLHC compass module, with tilt adjustment
* DisplayLogger - logs to the OLED display
* Globe - lat/long calculations  
* Gps - wrapper for the Adafruit GPS Library which enables/disables the GPS, and gets a reading within a specified time (if possible)
* Helm - Steer a direct course and adjust the sail
* I2C - wrapper for the Wire library, with error handling
* IHelm - interface for Helm implementations
* Logger - Abstract class for logging (implemented by specific loggers)
* MServo - wrapper for the Pololu Maestro servo board library
* MultiLogger - logs simultaneously to the given list of loggers
* Navigator - Navigate to a destination lat/long by repeatedly steering
* Position - lat/long struct with error values
* RotaryPID - a PID controller for the rudder using angles rather than a linear scale (see https://en.wikipedia.org/wiki/PID_controller)
* Route - Interface for a route object (like an iterator for a route)
* Routes - waypoint data for navigating
* Rudder - wrapper around servo to limit deflection
* Sail - sailing logic; set sail angle according to relative wind, only gybe when necessary
* SDBackedRoute - Route which keeps progress in a file on the SD card
* SDLogger - logs to an SD card
* SelfTest - power-on self test routines
* SerialLogger - log to the serial port (for Arduino Serial monitor)
* Stubs - home grown stubs for use in unit testing
* Switches - reads three switches for in-the-field configuration changes
* Tacker - steer directly or do a tack, using Helm
* TankHelm - a version of Helm for tank testing
* TestEg - example of using Gtest (google C++ test library)
* Timer - wrapper around delay function, for testing and possible multi-tasking
* Utility - templates for common functions, unix time
* Waypoint - waypoint definition
* Windrange - data structure to hold ranges of acceptable wind direction, to determine if the current course should be abandoned
* WindSensor - wrapper around the AS5048B 14 bit rotary position sensor, to return a relative wind angle between -180 and +180

## Field tests
See the separate [field report page](https://github.com/andyrobinson/ard-nav/blob/master/FieldReports.md).

## Satellite Communications
Notes & Questions:

1.  Currently no signal received under any circumstances.  In the process of debugging with RockBlock
2.  The library by default tries to send a communication for 300 seconds (5 mins).  In between send attempts it waits on a loop for 10 seconds 
    calling a predefined call back (returns a boolean to indicate cancelled) without delay.  The expectation is that the call back will be
    overridden.
3.  10 seconds is significant because this is about the time it should take for the capacitors to recharge at 0.5A

Concerns

* thirty (30) attempts with constant battery charging at 0.5A will take quite a lot of power.  The overall budget is 0.1A constant (2.4Ah @4v = 9.6Wh).  
The batter capacity is 13Ah = 52Wh or about 5 days).  Each transmission event will take 0.5 x 0.0833 x 5 = 0.21Wh or approximately one tenth of the budget.  Given we are expecting
  four logging events in 24 hours thats about 0.4 of the entire power budget.
* If we implement exponential backup then we reduce this to 5 or 6 attempts in 5 minutes or about 1/5 of the budget, but increases the risk of a missed 
  communication.  Given that the gap between satellites is about 9 minutes (11 satellites evenly spread around a 100 minute orbit) then 5 minutes is probably 
  a reasonable transmission window.  We should do this as a minimum, probably by modifying the library.
* there is an opportunity to use the callback for steering in the longer windows (and/or reducing power by sleeping).  We should wait for 15 seconds before 
attempting to operate the servos so that the satellite module has recharged, so this probably means first time we do nothing
* the tricky part is ensuring that the callback has enough information to perform the navigation - suggests that we redefine the callback in nav.ino
* the need to define a single callback means that we shouldn't add satellite as a normal logger - we would need to insert it at a particular point

Implementation

Begin
- store the start-up time
- zero all values
- turn off module

msg - call main
banner - call main
write_version - ignore
setdest - store
settack - ignore

main
* we have to match on a specific time, because if there are multiple restarts per hour then either we will log too often or not at all.  However care needs to be taken because of sleep periods - may require that there is a special signal for checking if it's time to log after sleep.
* If real time is not available from the GPS then it should be from the satellite module - if not we can't transmit anyway!

* let's assume that we specify
** The hours that we want to log (in an array e.g [0,6,12,18])
** The minutes of the hour that we want to log (in an array e.g. [15,45])
** As a constant the logging window in minutes
* Note that when we log we need to wait (sleep) out the remainder of the logging window before resuming, so that there's no possibility of double logging
* If real time is not available from the GPS then it should be from the satellite module - if not we can't transmit anyway!

if it's not time to log, then just update the values and carry on.  values to update:
* min battery voltage
* min available memory
* GPS co-ordinates, in case not known at time of logging
* timestamp of last good position

otherwise
- put the data in an array as binary data
- attempt to send (I think this is on a 5 min timeout)
- log the result
- have a separate satellite log file on the SD card, and just comment out the actual send most of the time
- zero the voltage and memory
- wait until the end of the logging window
- consider if we need to use the callback while transmitting (for what?).  The danger is that the power supply will be overwhelmed, causing a brownout

A few notes on data
* GPS co-ordinates as fixed point (4 byte integers)
* Battery voltage as unsigned short (2 bytes) (2.00v - 4.55v) (/)
* memory should be possible in short or unsigned short (/)
* Last restart should be unix time which is a long

Note 6 hourly reports are required.  If the battery is very low we might reduce this to every 12 or 24 hours, but this will result in disqualification; the problem with this is that two are required in the hours of darkness

## Planned development

What do we need to send back in satellite data?
- GPS co-ordinates
- Current waypoint we're sailing towards
- Battery health (and history?)
- GPS Fix
- I2C errors
- free memory (max/min)
- last restart
- any other health information? ideally hull breach

Need to think about how to handle at sea restarts, which will be inevitable
- should we preserve state on the SD card?  
  SD card reading is pretty crude, so would need to read until the end-of-line
  The issues are
  - How to reset the state at a legitimate restart - does this mean doing something to the SD card which will be
    very inconvenient in the field
  - fallback if the SD card is not available - a restart with a failed card should
    not be catastrophic
  - How to manage the periodical sending of satellite information with a failed
    SD card and possible frequent restarts - probably the only thing we should
    rely on is the GPS clock, and perhaps not even this.  We need some system
    that guarantees we communicate, but not too often as this will drain the
    battery
    
## Further notes on error handling/recovery
- where possible the system should function if a single component fails, with the exeption of essential components:
  - Non recoverable:
    Rudder, Sail, Arduino, Battery & Solar charging
  - Recoverable
    Arduino crash, arduino restart
    SD Card, Satellite comms (although boat is now lost), compass, GPS (head on fixed bearing), Windsensor (tricky)
- where there is multiple component failure the behaviour is undefined (otherwise this is going to be very mad to code around)
    
- need to calculate the next nearest waypoint, not start from the beginning again every time!  This is not
as simple as the nearest in distance, we also need to think about the bearings of
  the nearest waypoint and the one after that.

## Future checks
1. Should introduce a deliberate memory leak to check that the memory measure works
2. Need some kind of integration testing
3.  We need follow-on code for failed sensors - wind direction (can we find out by steering?), compass (use GPS), gps (use dead reckoning).  Maybe ultimately we need more than one sensor.
4.  Note that when the input voltage falls below 6v that spikes caused by servo operation cause the Arduino to 
    crash - this may have happened during recent OTW testing.  We need to ensure that the Arduino power supply is protected, and there is a fall-back reset.  Ideally when the batteries get low we shut down until they regain some charge (I guess this could be never ...).  There are voltage regulators 
    that will ensure a stable voltage for as long as possible.
5.  Don't forget fallback (watchdog) timer which reboots the arduino after a period of inactivity (aka crash)
6.  Need to review all limits (e.g. max steer time) before attempting longer journeys
7. Other checks to Consider
- in irons
- becalmed (ensure preservation of battery)
- in a storm (ditto)

## Done (for reference)

-  In the final attempt we need to disable self test (probably completely), to take account of the fact that the system may restart during navigation.  Also the navigation needs to identify the nearest waypoint when it starts, and determine if it has passed this waypoint before selecting a waypoint to navigate towards.
- To abandon a tack, or start tacking if wind requires it
- Compass is highly influenced by electric motors - need to check in-situ and potentially isolate - much better outdoors, still perhaps 5 degrees out in the box, so we need to check in the boat
- It would be good to use the analog inputs to measure the battery voltage.  Note that once we have a proper power supply this will require a connection directly from the battery
- Logging now includes memory
- Need better indication from selftest that everything is well, so that we can be sure that nav has started when we no longer have the display
- Rewrite functions which return structs to using pointer arguments, for efficiency and
more importantly memory conservation
- software versioning
- average speed

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
