//
//  Random.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "Random.hpp"

#include <cstdlib>

using namespace nautical;

void Random::initRand(int seed) {
    srand(seed);
}

int Random::getRandInt(int range, int lowerBound) {
    return (rand() % range) + lowerBound;
}

float Random::getRandFloat(float range, float lowerBound) {
    return ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * range) + lowerBound;
}

bool Random::getRandBool(float trueOdds) {
    if (trueOdds >= 1)
        return true;
    else if (trueOdds <= 0)
        return false;
    
    return (getRandFloat() <= trueOdds);
}
