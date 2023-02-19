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



