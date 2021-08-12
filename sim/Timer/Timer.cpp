#include "Timer.h"

Timer::Timer(Boat *boatp): boat(boatp), theTime(0) {}


unsigned long Timer::elapsed() {
    return theTime;
}

void Timer::wait(unsigned long milliseconds) {
    theTime = theTime + milliseconds;
}
