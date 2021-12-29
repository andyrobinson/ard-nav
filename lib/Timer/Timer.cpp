#include "Timer.h"
#include "Arduino.h"
//#include "Adafruit_SleepyDog.h"

// to allow mocking of delays.
// Also potentially we could do useful work while waiting in the future
Timer::Timer() {}

void Timer::wait(unsigned long milliseconds) {
  //Watchdog.reset(); // note we need to call the timer from the GPS module as well
  delay(milliseconds);
}
