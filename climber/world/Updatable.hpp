//
//  Updatable.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Updatable_hpp
#define Updatable_hpp

#include <memory>

namespace nautical {
    class World;
    
    class Updatable {
    public:
        int getTimestamp() const { return timestamp; }
        
        void updateTimestamp(int timestamp, World * p_world, double dt = 1) {
            if (this->timestamp != timestamp) {
                this->timestamp = timestamp;
                update(p_world, dt);
            }
        }
        
        virtual void update(World * p_world, double dt = 1) = 0;
        
    private:
        int timestamp = -1;
    };
}

#endif /* Updatable_hpp */
