#include "Timer.h"

// to allow mocking of delays.
// Also potentially we could do useful work while waiting in the future
Timer::Timer() {}

void Timer::wait(unsigned long milliseconds) {
  // do nothing at the moment
}
