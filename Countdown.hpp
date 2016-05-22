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
        Countdown(int set = 0, int loops = -1);
        
        bool check();
        Countdown & reset();
        Countdown & reset(int set);
        int getLoopCount() const;
        Countdown & addLoops(int loops);
        
    private:
        int count,
        set,
        loopCount,
        maxLoops;
    };
}

#endif /* defined(__Nautical__Countdown__) */
