//
//  Countdown.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Countdown.hpp"

using namespace nautical;

Countdown::Countdown(int set, int loops) :
count(set),
set(set),
loopCount(0),
maxLoops(loops) { }

bool Countdown::check() {
    if (maxLoops - loopCount >= 0) {
        if (!(--count > 0)) {
            if (maxLoops >= 0)
                loopCount++;
            reset();
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

Countdown & Countdown::reset() {
    return reset(set);
}

Countdown & Countdown::reset(int set) {
    this->set = set;
    count = set;
    return *this;
}

int Countdown::getLoopCount() const {
    return loopCount;
}

Countdown & Countdown::addLoops(int loops) {
    if (maxLoops < 0) {
        maxLoops = loops;
    } else {
        maxLoops += loops;
    }
    return *this;
}