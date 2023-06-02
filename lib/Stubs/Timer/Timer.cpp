#include "Timer.h"

// to allow mocking of delays.
// Also potentially we could do useful work while waiting in the future
Timer::Timer():timer_millis(0),is_set(false) {}

void Timer::wait(unsigned long milliseconds) {
  timer_millis += milliseconds;
}

unsigned long Timer::milliseconds()  {
  return timer_millis;
}

void Timer::set_millis(long millis) {
  timer_millis = millis;
}

// time functions
time_t Timer::now() {
    return set_time + timer_millis/1000;
}

tm *Timer::nowTm() {
    time_t t = now();
    return gmtime(&t);
}

void Timer::setTime(time_t t) {
    set_time = t;
    is_set = true;
}

bool Timer::isTimeSet()  {
    return is_set;
}

