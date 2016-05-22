//
//  Clock.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Clock.hpp"

#include <sys/time.h>

using namespace nautical;

Clock::Clock() {
    delta();
}

long int Clock::delta() {
    long int currentTime = wallTime();
    long int delta = currentTime - prevTime;
    prevTime = currentTime;
    return delta;
}

long int Clock::split() const {
    return wallTime() - prevTime;
}

long int Clock::wallTime() {
    timeval currentTime;
    gettimeofday(&currentTime, nullptr);
    return (currentTime.tv_sec * 1000) + (currentTime.tv_usec / 1000); //converts seconds and microseconds to milliseconds
}
