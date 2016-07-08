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

void Random::initRand(unsigned int seed) {
    srand(seed);
}

int Random::getRandInt(int range, int lowerBound) {
    return (rand() % range) + lowerBound;
}

double Random::getRandDouble(double range, double lowerBound) {
    return ((static_cast<double>(rand()) / static_cast<double>(RAND_MAX)) * range) + lowerBound;
}

bool Random::getRandBool(float trueOdds) {
    if (trueOdds >= 1)
        return true;
    else if (trueOdds <= 0)
        return false;
    
    return (getRandDouble() <= trueOdds);
}
