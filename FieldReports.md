# Field Reports

### 11 March 2023, Workshop
* Software - try to isolate the problem - means using the outer loop, binary chop on rudder limit
- slightly noisy?
** Good 20, 30, 35
** Bad 40
* Hardware - try a different channel (if possible)


### 8 March 2023, Workshop
* This is with Solar charging working ...
* Still getting problems with crashing and the Rudder.  Always happens at the right-hand end of travel (most negative)
* Crashing patterns not consistent, but there could be interaction between Sail and Rudder
* Rotary PID does not seem to be implicated ... however I thought this worked if we ignored all navigation
* Removing the Speed and Accel from Rudder does not help
* Try reducing Rudder range to 20 degrees [seems to fix, but rudder sweep is not symmetrical]
* Try moving loop to outer nav, at 40 degrees [consistently resets, but keeps going, seems to also activate sails]
* Try using rotary PID at 20 degrees [does NOT crash]
[Conclusion - there's something up with the rudder control, could be software or hardware]


### 8 Jan 2023, Garden
Garden test allows testing with GPS and Solar panel working.  Inside neither of these elements is working correctly
* System consistently crashes.  Initially it would just crash after the first log message.
* Removed buffering of banner log messages (writing them straight to file), meant that the system would run for about 2 mins before crash
* Reverted to 21 November version of software, which previously ran in a field test for 50 mins without problem, but still crashes after approximately 40 seconds
* Turning on the RC unit (which gives a more predictable analog input for the Rotary PID module) resulted in a longer run of 2 mins
* Running inside resulted in 7.51 run before crash.  Interestingly perhaps rotating the boat left 90 degrees (so that the steering changed) seemed to precipitate the crash.  Will try that again.
* Working fine after 5 minutes, now try changing direction ....  rotation through 90 degrees immediately precipitates a crash.   Could be compass, rotary PID or rudder servo.
* Tracked this down to a servo glitch at the far range of the rudder, reduced rudder to eliminate

### 8 Jan 2023, Platt Fields Park

Restarts
* There were 3 unplanned restarts in the first 212 seconds.  Note the timestamp for the first start is longer as the GPS has to get a fix.
* Timings were after 36 seconds, then 40 seconds, then 36 seconds.
* Other restarts at 1107, 3000 (intentional, change of route), 3103, 5023, 5114
* This is NOT consistent with a low battery problem - you would expect that after the initial restarts it would settle down
* Could it be temperature related?  It was a very cold day
* It may help to record minimum voltages as well as maximum, to see if brown-out is causing this issue
* Note also this is different from the original crash problems - the system restarts and functions without problems, for up to 50 minutes, but it's worth re-reading the notes from this time.
* Given the number of resets in the initial phase it should be easy to reproduce(?!)

Number of abandons, approach to tacking
* There are significant numbers of abandons, even though the strategy of moving to the second tack should give a good angle on the wind if the first tack fails
* Typically the infringement is only a few degrees - if we relax both the angle of tack and the wind range this should reduce this
* There's no evidence that the wind direction used to decide on the tack is wildly out (suggesting the need for an average wind) 
* It may also reflect a slightly unresponsive wind direction sensor, so it doesn't fully swing round to reflect the new wind direction

Steering constants
* A 0 percent adjustment was found to be most effective
* See if this can be adjusted further, i.e. move the 0 percent up to the 50 percent

#### Actions
* Investigate the voltage behaviour on start-up using the scope
  This does NOT appear to be a problem with voltage fluctuation - the solar power seems to be completely stable.

- resetting software and trying repeated runs of 5 minutes, inside and outside (with working compass and solar
  - inside (battery only) OK
  - inside (solar active 1) - no logs after 85 seconds, no sign of a restart - actually OK, logged elsewhere
  - inside (solar active 2) - accesses SD card every 10 seconds.  All logs are fine - somehow it's figured out the time even though it has no fix

  - outside (solar)
    - does NOT work.  Only first value is logged, suggests software problem with logger - also erratic behaviour suggests buffer overrun problem.  Have changed the itoa call to ltoa (as this is correct), and removed buffering of log messages

* Calculate and log minimum voltage values as well as max (/)
* Relax the tack and the wind range (/)
* Adjust the steering constant range (/)


### 13 May, Land based
* The rudder seems to make for weaving navigation - in part due to the unresponsiveness of a human vessel
* We need a better estimate of speed (and therefore time to destination).  If we underestimate the speed then we will overshoot the target.  Points towards moving averages for both speed and absolute wind direction (and therefore a regular update to both, probably via the GPS interrupt timer)
* the logging could be better - where are we heading, how long before next review, are we tacking, and is it tack1 or tack2?
* compass is much better outside
* broadly speaking the software appears to work(!)

### 25 April 2021, on the water
* Seems very keen to tack
* Steering makes the boat weave
* Logging stopped after 10 mins of second run - will add capacitor to try and remove spikes, but need to monitor
