//
//  Iterator.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/15/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef __Nautical__Iterator__
#define __Nautical__Iterator__

namespace nautical {
    template<typename T>
    class Iterator { //abstract class for all iterators
    public:
        Iterator(bool selfDestruct = true) : selfDestruct(selfDestruct) { }
        virtual ~Iterator() { }
        
        bool shouldSelfDestruct() const { return selfDestruct; }
        
        virtual Iterator<T> * first() = 0;
        virtual Iterator<T> * next() = 0;
        virtual bool complete() const = 0;
        virtual T current() const = 0;
        
    protected:
        bool selfDestruct;
    };
}

#endif /* defined(__Nautical__Iterator__) */
