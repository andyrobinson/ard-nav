# Simulation
Simulation to test out bigger aspects of the sailing algorithm which cannot easily be done by field testing, specificiatlly:

* Effect of tides
* Drift in strong winds
* Confirm operation over longer distances, across the equator etc.
* Correct resuming of route after a crash
* Deciding when to skip a waypoint

## Implemetation
As far as possible the original unmodified libs are used.  Libs encapusulating hardware sensors and servos are 
replaced with libs than wrap a central "boat" which contains a simplified physics engine.  When the navigation software
requests to sleep, the simulation calculates the new position for the boat and returns immediately.

## To do
* Make the boat respond to the wind direction (currently moves forwards regardless)
* Add forward momentum (slowish decay)
* Add angular momentum to turns (faster decay)
* Make the boat respond to the wind strength up to maximum speed
* Make strong side winds reduce the effectiveness of the boat (excessive heel)
* Add downwind drift, amount varying with relative direction (max when abeam)
* Allow the wind to vary in strength and direction over time
* Add tides - n.b. period - about 12 hours and 25 mins
* Real dates?
* How to easily plot the simulation?

