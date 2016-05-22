//
//  Random.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Random__
#define __Nautical__Random__

namespace nautical {
    class Random {
    public:
        static void initRand(int seed);
        static int getRandInt(int range, int lowerBound = 0);
        static float getRandFloat(float range = 1, float lowerBound = 0);
        static bool getRandBool(float trueOdds = 0.5); //returns a boolean with trueOdds chance of true and 1 - trueOdds chance of false
    };
}

#endif /* defined(__Nautical__Random__) */
