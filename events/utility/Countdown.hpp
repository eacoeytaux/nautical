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
    class Countdown { //TODO when countdown finishes does it always return true?
    public:
        Countdown(double set = 0, int loops = -1);
        
        bool check(double checks = 1);
        Countdown & reset();
        Countdown & reset(double set); //resets countdown and sets count to new set
        int getLoopCount() const;
        Countdown & addLoops(int loops); //adds number of times countdown will restart before stopping
        
    private:
        double count, //current countdown value
        set; //value count will reset to when reaching 0
        int loopCount, //number of times count has reached 0
        maxLoops; //number of times countdown will restart before stopping
    };
}

#endif /* defined(__Nautical__Countdown__) */
