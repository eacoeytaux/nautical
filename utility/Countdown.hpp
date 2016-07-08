//
//  Countdown.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Countdown__
#define __Nautical__Countdown__

namespace nautical {
    class Countdown {
    public:
        Countdown(double set = 0, int loops = -1);
        
        bool check(double checks = 1);
        Countdown & reset();
        Countdown & reset(double set);
        int getLoopCount() const;
        Countdown & addLoops(int loops);
        
    private:
        double count,
        set;
        int loopCount,
        maxLoops;
    };
}

#endif /* defined(__Nautical__Countdown__) */
