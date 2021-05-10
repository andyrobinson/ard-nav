#include "Timer.h"

Timer::Timer() {
    theTime = 0;
}

unsigned long Timer::elapsed() {
    return theTime;
}

void Timer::wait(unsigned long milliseconds) {
    theTime = theTime + milliseconds;
}
