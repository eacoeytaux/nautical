//
//  Clock.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/17/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Clock__
#define __Nautical__Clock__

namespace nautical {
    class Clock {
    public:
        Clock();
        
        long int delta(); //returns number of milliseconds since last delta call (or constructor) and resets delta
        long int split() const; //returns number of milliseconds since last delta call, without resetting delta
        
        static long int wallTime(); //returns number of milliseconds from Jan 1, 1970, 00:00:00
        
    private:
        long int prevTime = 0; //last delta() call in milliseconds
    };
}

#endif /* defined(__Nautical__Clock__) */
