#include "Timer.h"
#include "Arduino.h"
//#include "Adafruit_SleepyDog.h"

// to allow mocking of delays.
// Also potentially we could do useful work while waiting in the future

Timer::Timer():isSet(false),lastSetMillis(0){}

void Timer::wait(unsigned long millisecs) {
  //Watchdog.reset(); // note we need to call the timer from the GPS module as well
  delay(millisecs);
}

unsigned long Timer::milliseconds() {
  return millis();
}

time_t Timer::now() {
    if (!isSet) return 0;
    return lastSetTime + (millis() - lastSetMillis)/1000;
}

tm *Timer::nowTm() {
    time_t t = now();
    return gmtime(&t);
}

void Timer::setTime(time_t t) {
    lastSetMillis = millis();
    lastSetTime = t;
    isSet = true;
}

bool Timer::isTimeSet() {
    return isSet;
}
