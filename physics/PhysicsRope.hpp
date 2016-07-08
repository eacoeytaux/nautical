//
//  PhysicsRope.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/4/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef PhysicsRope_hpp
#define PhysicsRope_hpp

#include <vector>

#include "Mass.hpp"
#include "Spring.hpp"

namespace nautical {
    namespace physics {
        class PhysicsRope {
        public:
            PhysicsRope(Coordinate anchor, double length, double segmentLength = 2);
            ~PhysicsRope();
            
            PhysicsRope & setAnchor(const Coordinate & anchor);
            
            void update();
            
            void draw() const; //TODO delete
            
        private:
            std::vector<Mass*> masses;
            std::vector<Spring*> springs;
        };
    }
}

#endif /* PhysicsRope_hpp */
