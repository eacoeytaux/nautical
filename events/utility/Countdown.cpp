//
//  Countdown.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Countdown.hpp"

using namespace nautical;

Countdown::Countdown(double set, int loops) :
count(set),
set(set),
loopCount(0),
maxLoops(loops) { }

bool Countdown::check(double checks) {
    if (set <= 0) {
        return true;
    } else if (count > checks) {
        count -= checks;
        return false;
    } else {
        checks -= count;
        loopCount++;
        while (checks >= set) {
            checks -= set;
            loopCount++;
        }
        reset();
        count -= checks;
        return true;
    }
}

Countdown & Countdown::reset() {
    return reset(set);
}

Countdown & Countdown::reset(double set) {
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
