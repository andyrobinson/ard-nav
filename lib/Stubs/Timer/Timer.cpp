#include "Timer.h"

// to allow mocking of delays.
// Also potentially we could do useful work while waiting in the future
Timer::Timer():timer_millis(0) {}

void Timer::wait(unsigned long milliseconds) {
  timer_millis += milliseconds;
}

long Timer::millis()  {
  return timer_millis;
}

void Timer::set_millis(long millis) {
  timer_millis = millis;
}
