//
//  Updatable.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Updatable_hpp
#define Updatable_hpp

namespace nautical {
    class Updatable {
    public:
        int getTimestamp() const { return timestamp; }
        
        void update(int timestamp) {
            if (this->timestamp != timestamp) {
                this->timestamp = timestamp;
                update();
            }
        }
        
        virtual void update() = 0;
        
    private:
        int timestamp = -1;
    };
}

#endif /* Updatable_hpp */
