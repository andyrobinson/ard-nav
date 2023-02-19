# Notes on Feb 2023 crashing behaviour

## Run 1 - 1.57
* Modifications: Using HEAD version with wait for GPS reduced to 1s
* Crashes on Rotation 90 degrees left

## Run 2 - 0.53
* Modifications: Replace Rotary PID with simple sweep
* Crashes as soon as the rudder reaches it's end point - suggests rudder is the problem

## Run 3
* Modifications: Replace all navigation with a rudder sweep
* At right-hand (negative) end of sweep, system crashes


