//
//  Rope2.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Rope2_hpp
#define Rope2_hpp

#include "WorldObject.hpp"

#include <vector>

namespace climber {
    class Rope2 : public nautical::WorldObject {
    public:
        Rope2(nautical::Coordinate anchor, double length);
        virtual ~Rope2();
        
        void update();
        
        void draw() const;
        
    private:
        struct RopeJoint;
        
        std::vector<RopeJoint*> joints;
    };
}

#endif /* Rope2_hpp */
