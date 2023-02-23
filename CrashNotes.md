# Notes on Feb 2023 crashing behaviour

## Run 1 - 1.57
* Modifications: Using HEAD version with wait for GPS reduced to 1s
* Crashes on Rotation 90 degrees left

## Run 2 - 0.53
* Modifications: Replace Rotary PID with simple sweep
* Crashes as soon as the rudder reaches it's end point - suggests rudder is the problem

## Run 3 - 0.26
* Modifications: Replace all navigation with a rudder sweep
* At right-hand (negative) end of sweep, system crashes, and starts to operate the sail as well

## Run 4 - 5.00
* Modifications: Reduce the rudder sweep to 35 degrees in each direction
* Seems to work fine, no problems

## Run 5 
* Modifications: Try increasing the limit, log the results on the serial monitor
* Runs out of iterations before limit!, try with more iterations

## Run 6
* Modifications: Try 20 iterations
* Seems to work past the limit - perhaps the logging is helping it?

## Run 7
* Modifications: Shorter pauses, so quicker, no logging of pulse widths
* Still works up to 49 degrees - need to go back to something that doesn't work

## Run 8 
* Return to failing scenario, run 3
* Still works with a 100ms delay, but there is an odd twitch at the RH end - longer delay - trying 500ms
* Still works

## Run 9
* Modifications: Reduce pause time to 50ms but with a long wait at the limit, gradually increasing limit
* Twitch at RH end, after some time appears to crash, but need to confirm with more definite start-up routine.
* Strange cross-over with the sail servo

## Run 10
* Modifications: Reduce pause time in sweep again, keep limit increase, do sail movement at start
* Maybe multiple crashes, got sail position wrong - it responds to the wind

## Run 10
* Modifications: Clear startup signal
* Does not crash, however the servo behaves erratically and there appears to be some cross-over between rudder and sail

## Run 11
* Modifications: Return to Rudder sweep in Helm
* System no longer crashes, running with USB plugged in
* System immediately crashes if the USB is not plugged in (suggests that something is causing a power glitch - probably the twitch)
* All future tests must be without the USB plugged in




